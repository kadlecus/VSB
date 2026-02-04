
using Database;
using IGasolineEngineDatabase;

namespace FillingStationDatabase
{
    public class FillingStation 
    {
        private double price;
        public FillingStation(double newPrice)
        {
            price = newPrice;

        }

        public void Refuel(IGasolineEngine vehicle)
        {
           double fuelToRefuel = 0;
            
            if (vehicle is Car car) { 
                double newPrice = this.price + 4 * car.NumberOfWheels();

                double maxFuelAffordable = car.AccountBalance / newPrice;
                double remaining = vehicle.FuelTankSize - vehicle.AmountOfFuel;

                fuelToRefuel = Math.Min(maxFuelAffordable, remaining);
                
                car.AccountBalance = car.AccountBalance - (fuelToRefuel * newPrice);
                
                //return fuelToRefuel;

                
            }else if (vehicle is Boat boat)
            {
                double newPrice = this.price + 4 * boat.NumberOfWheels();

                double maxFuelAffordable = boat.AccountBalance / newPrice;
                double remaining = vehicle.FuelTankSize - vehicle.AmountOfFuel;

                fuelToRefuel = Math.Min(remaining, maxFuelAffordable);

                boat.AccountBalance = boat.AccountBalance - (fuelToRefuel * newPrice);
                
               // return fuelToRefuel;
            }
            vehicle.Refuel(fuelToRefuel);
           // return 0;
        }
    }
}
