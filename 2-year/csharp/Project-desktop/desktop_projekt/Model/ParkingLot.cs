using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace desktop_projekt.Model
{
        public class ParkingLot
        {
            private int id;
            public int Id
            {
                get
                {
                    return id;
                }

                set
                {
                    if (0 > value)
                    {
                        throw new ArgumentOutOfRangeException(nameof(Id), "Index out of range.");
                    }
                    id = value;

                }
            }
            public string Name { get; set; }

            private double latitude = 0;
          
            public double Latitude
            {
                get
                {
                    return latitude;
                }
                set
                {
                    if (value < -90 || value > 90)
                    {
                        throw new ArgumentOutOfRangeException(nameof(Latitude), "Latitude must be between -90 and 90 degrees.");
                    }
                    latitude = value;
                }
            }
            private double longitude = 0;
          
            public double Longitude
            {
                get
                {
                    return longitude;
                }
                set
                {
                    if (value > 180 || value < -180)
                    {
                        throw new ArgumentOutOfRangeException(nameof(Latitude), "Longitude must be between -180 and 180 degrees.");
                    }
                    longitude = value;
                }
            }

         
            public int FreeSpots { get; set; }



        }
    }


