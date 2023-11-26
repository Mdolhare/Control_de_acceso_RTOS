import sys
import urllib.request

myAPI = 'FOJQUWVWCX7IHVU1'
baseURL = 'https://api.thingspeak.com/update?api_key=%s' % myAPI

conn = urllib.request.urlopen(baseURL + '&field2=%s&field3=%s' % (100,400))
print(conn.read())

conn.close()