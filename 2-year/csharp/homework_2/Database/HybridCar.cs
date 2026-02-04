using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IGasolineEngineDatabase;
using IElectricEngineDatabase;

namespace Database
{
    public class HybridCar : Car, IGasolineEngine, IElectricEngine
    {
        public double BatteryCapacity { get; init; }
        public double RemainingEnergy { get; private set; }
        public double AmountOfFuel { get; private set; }
        public double FuelTankSize { get; init; }


       

        public void Charge(double quantity)
        {
            this.RemainingEnergy += quantity;
            if (this.RemainingEnergy > this.BatteryCapacity)
            {
                this.RemainingEnergy = this.BatteryCapacity;
            }

        }

        public void Refuel(double quantity2)
        {
            this.AmountOfFuel += quantity2;
            if (this.AmountOfFuel > this.FuelTankSize)
            {
                this.AmountOfFuel = this.FuelTankSize;
            }
        }
        public override void DisplayInfo()
        {
            base.DisplayInfo();
            
            Console.WriteLine("Stav baterie: " + Math.Round((this.RemainingEnergy / this.BatteryCapacity) * 100 ) + "% " + " | " + "Stav nádrže: " + Math.Round((this.AmountOfFuel / this.FuelTankSize) * 100) + "%\n");   
        }



   

    }
}
