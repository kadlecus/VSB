
using Database;
using IElectricEngineDatabase;

namespace ChargingStationDatabase
{
    public class ChargingStation
    {
        private double price;
        public ChargingStation(double newPrice)
        {
            price = newPrice;

        }

        public void Refuel(IElectricEngine vehicle)
        {

            double fuelToRefuel = 0;

            if (vehicle is Car car)
            {
                double newPrice = this.price + 4 * car.NumberOfWheels();

                double maxFuelAffordable = car.AccountBalance / newPrice;
                double remaining = vehicle.BatteryCapacity - vehicle.RemainingEnergy;

                fuelToRefuel = Math.Min(maxFuelAffordable, remaining);

                car.AccountBalance = car.AccountBalance - (fuelToRefuel * newPrice);

              //  return fuelToRefuel;


            }
            else if (vehicle is Boat boat)
            {
                double newPrice = this.price + 4 * boat.NumberOfWheels();

                double maxFuelAffordable = boat.AccountBalance / newPrice;
                double remaining = vehicle.BatteryCapacity - vehicle.RemainingEnergy;

                fuelToRefuel = Math.Min(remaining, maxFuelAffordable);

                boat.AccountBalance = boat.AccountBalance - (fuelToRefuel * newPrice);
                
               // return fuelToRefuel;
            }
            //return 0;
            vehicle.Charge(fuelToRefuel);
        }

    }
}
