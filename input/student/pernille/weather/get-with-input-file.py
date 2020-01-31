import cdsapi
import os
import pandas

os.chdir('C:\\Users\\au152367\\Documents\\QDev\\UniSim2\\input\\student\\pernille\\weather')

locations = pandas.read_csv('brazil_locations.txt', sep='\t')
locations.head()

client = cdsapi.Client()

def f(latitude, longitude, year):
  for month in range(1,13):
    if (month<10):
      month2 = '0' + str(month)
    else:
      month2 = str(month)
    fileName =  str(latitude) + "_" + str(longitude) + "_" + str(year) + "_" + str(month2)
    area = str(latitude) + "/" + str(longitude) + "/" + str(latitude) + "/" +str(longitude)
    print(fileName+"...")
    client.retrieve(
      'reanalysis-era5-land',
      {
        'variable': [
          '2m_dewpoint_temperature', '2m_temperature', 'surface_pressure',
          'surface_solar_radiation_downwards', 'total_precipitation',
        ],            
        'area': area,
        'year': str(year), 
        'month': month2,
        'day': [
          '01', '02', '03', '04', '05', '06',
          '07', '08', '09', '10', '11', '12',
          '13', '14', '15', '16', '17', '18',
          '19', '20', '21', '22', '23', '24',
          '25', '26', '27', '28', '29', '30',
          '31'
        ],            
        'time': [
          '00:00', '01:00', '02:00',
          '03:00', '04:00', '05:00',
          '06:00', '07:00', '08:00',
          '09:00', '10:00', '11:00',
          '12:00', '13:00', '14:00',
          '15:00', '16:00', '17:00',
          '18:00', '19:00', '20:00',
          '21:00', '22:00', '23:00',
        ],
        'format'  : 'netcdf',
      }
      , 
      '/Users/au152367/Documents//QDev/UniSim2/input/student/pernille/weather/nc/' + fileName + '.nc'
    )
  return;

for index, row in locations.head().iterrows():
  f(row['Latitude'], row['Longitude'], row['Year'])

