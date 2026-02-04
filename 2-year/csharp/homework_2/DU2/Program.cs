using System;
using System.Globalization;
using System.Net.Http.Headers;
using Database;
using FillingStationDatabase;
using ChargingStationDatabase;
using IGasolineEngineDatabase;
using IElectricEngineDatabase;




namespace MyApp // Note: actual namespace depends on the project name.
{
    internal class Program
    {

        static void Refuel(ChargingStation chargingStation, FillingStation fillingStation, Vehicle[] vehicles)
        {

            foreach (Vehicle vehicle in vehicles) 
            {

                if (vehicle is IGasolineEngine a)
                {
                        fillingStation.Refuel(a);
                       // a.Refuel(quantity);
                }
                
                if (vehicle is IElectricEngine b)
                {
                    chargingStation.Refuel(b);
                  //  b.Charge(quantity);

                }
            }

        }
        static void Main(string[] args)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("cs-CZ");
           
            
            
            GasolineCar vehicle1 = new GasolineCar()
            {
                VehicleName = "Benzínové auto",
                AccountBalance = 2030,
                FuelTankSize = 45
            };
            vehicle1.Refuel(10);

            ElectricCar vehicle2 = new ElectricCar()
            {
                VehicleName = "Elektrické auto",
                AccountBalance = 2000,
                BatteryCapacity = 80
            };
            vehicle2.Charge(10);


            HybridCar vehicle3 = new HybridCar()
            {
                VehicleName = "Hybrid",
                AccountBalance = 2000,
                BatteryCapacity = 65,
                FuelTankSize = 30
            };
            vehicle3.Refuel(20);
            vehicle3.Charge(20);


            GasolineBoat vehicle4 = new GasolineBoat()
            {
                VehicleName = "Loď",
                AccountBalance = 1000,
                FuelTankSize = 30

            };
            vehicle4.Refuel(0);


            Vehicle[] vehicles = new Vehicle[]
            {
        vehicle1, vehicle2, vehicle3, vehicle4
            };

            // --
            
            
                        ChargingStation chargingStation = new ChargingStation(35.2);
                        FillingStation fillingStation = new FillingStation(12.5);
            
                        // -

                        foreach (Vehicle vehicle in vehicles)
                        {
                            vehicle.DisplayInfo();
                            Console.WriteLine();
                        }


                        Console.WriteLine();


                        Refuel(chargingStation, fillingStation, vehicles);
            

            Console.WriteLine("--------------------");
            Console.WriteLine("Po doplnění paliva:");
            Console.WriteLine();
            foreach (Vehicle vehicle in vehicles)
            {
                vehicle.DisplayInfo();
                Console.WriteLine();
            }
            
        }
    }
}