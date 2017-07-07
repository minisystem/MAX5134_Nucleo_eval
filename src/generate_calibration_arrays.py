# All the Google Sheets stuff is taken from https://developers.google.com/sheets/api/quickstart/python
from __future__ import print_function
import httplib2
import os

from apiclient import discovery
from oauth2client import client
from oauth2client import tools
from oauth2client.file import Storage

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()
except ImportError:
    flags = None

# If modifying these scopes, delete your previously saved credentials
# at ~/.credentials/sheets.googleapis.com-python-quickstart.json
SCOPES = 'https://www.googleapis.com/auth/spreadsheets.readonly'
CLIENT_SECRET_FILE = 'data/calibration_spreadsheet_access_secret.json'
APPLICATION_NAME = 'Calibration'


def get_credentials():
    """Gets valid user credentials from storage.

    If nothing has been stored, or if the stored credentials are invalid,
    the OAuth2 flow is completed to obtain the new credentials.

    Returns:
        Credentials, the obtained credential.
    """
    home_dir = os.path.expanduser('~')
    credential_dir = os.path.join(home_dir, '.credentials')
    if not os.path.exists(credential_dir):
        os.makedirs(credential_dir)
    credential_path = os.path.join(credential_dir,
                                   'sheets.googleapis.com-python-quickstart.json')

    store = Storage(credential_path)
    credentials = store.get()
    if not credentials or credentials.invalid:
        flow = client.flow_from_clientsecrets(CLIENT_SECRET_FILE, SCOPES)
        flow.user_agent = APPLICATION_NAME
        if flags:
            credentials = tools.run_flow(flow, store, flags)
        else: # Needed only for compatibility with Python 2.6
            credentials = tools.run(flow, store)
        print('Storing credentials to ' + credential_path)
    return credentials

def replaceFixedAreaWithText(fname, new_text, start_line="// PYTHON REPLACE HERE", end_line="// END PYTHON REPLACE HERE"):
  lines = []
  with open(fname, "r") as f:
    in_replace = False
    for line in f:
      if line.startswith(start_line):
        lines.append(line)
        lines.append(new_text)
        lines.append("\n")
        in_replace = True
      elif line.startswith(end_line):
        in_replace=False
      
      if not in_replace:
        lines.append(line)
  #print(''.join(lines))
  
  with open(fname, "w") as f:
    f.write(''.join(lines))


BASE_OFFSET_CODE = 'channel[%d].zero_offset = %d;'
INTERVAL_CODE = 'channel[%d].code_interval = %d;'
INDIVIDUAL_OFFSET_CODE = 'channel[%d].pitch_table[%d] = %d;'
  
def generate_c_code(meta_data_range):
    """
    """
    credentials = get_credentials()
    http = credentials.authorize(httplib2.Http())
    discoveryUrl = ('https://sheets.googleapis.com/$discovery/rest?'
                    'version=v4')
    service = discovery.build('sheets', 'v4', http=http,
                              discoveryServiceUrl=discoveryUrl)

    # TEST SHEET and RANGE
    #spreadsheetId = '1BxiMVs0XRA5nFMdKvBdBZjgmUUqptlbs74OgvE2upms'
    #rangeName = 'Class Data!A2:E'
   
    spreadsheetId = '1X307bgHHYvqqV0sM_jZDzlABrWuNk7hi44A3dp9biuI'
    rangeName = 'Sheet1!%s' % meta_data_range
    result = service.spreadsheets().values().get(
        spreadsheetId=spreadsheetId, range=rangeName).execute()
    values = result.get('values', [])

    # make sure we are looking at the right data
    assert values[0][0] == 'CODE_OFFSET'
    assert values[1][0] == 'CODE_INTERVAL'
    assert values[2][0] == 'NUM_CODES'
    assert values[3][0] == 'OFFSET_START'
    
    num_codes = int(values[2][1])
    
    # now let's get a list of each dataset we need to consider:
    raw_data = [values[0][1].split(','), values[1][1].split(','), values[3][1].split(',')] 
    datasets = list(map(list, zip(*raw_data)))
    
    channel = 0
    code = []
    for dataset in datasets:
      # get the offset, the interval, and individual offsets       
      offset_cell = dataset[0]
      rangeName = 'Sheet1!%s' % offset_cell
      result = service.spreadsheets().values().get(
          spreadsheetId=spreadsheetId, range=rangeName).execute()
      values = result.get('values', [])
      offset = int(values[0][0])
    
      interval_cell = dataset[1]
      rangeName = 'Sheet1!%s' % interval_cell
      result = service.spreadsheets().values().get(
          spreadsheetId=spreadsheetId, range=rangeName).execute()
      values = result.get('values', [])
      interval = int(values[0][0]) - offset
      
      # now we get the individual offsets
      offs_start = dataset[2]
      rangeName = 'Sheet1!%s:%s' % (offs_start, offs_start[0] + str(int(offs_start[1:]) + num_codes-1))
      result = service.spreadsheets().values().get(
          spreadsheetId=spreadsheetId, range=rangeName).execute()
      values = result.get('values', [])
      offs = list(map(lambda x: int(x[0]), values))
      
      code.append(BASE_OFFSET_CODE % (channel, offset))
      code.append(INTERVAL_CODE % (channel, interval))
      i = 0
      for off in offs:
        code.append(INDIVIDUAL_OFFSET_CODE % (channel, i, off))
        i += 1
      channel += 1
    return "\n".join(code)
    
    
if __name__ == '__main__':
    code = generate_c_code('H7:I10')
    replaceFixedAreaWithText("calibration.c", code)
    