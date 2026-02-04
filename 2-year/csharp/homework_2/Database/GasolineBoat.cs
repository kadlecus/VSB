using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IGasolineEngineDatabase;

namespace Database
{
    public class GasolineBoat : Boat, IGasolineEngine
    {
        public double AmountOfFuel { get; private set; }
        public double FuelTankSize { get; init; }

     
       
        public void Refuel(double quantity)
        {
            this.AmountOfFuel =+ quantity;
            if (this.AmountOfFuel > this.FuelTankSize)
            {
                this.AmountOfFuel = this.FuelTankSize;
            }
        }

        public override void DisplayInfo()
        {
            base.DisplayInfo();

            Console.WriteLine("Stav nádrže: " + Math.Round((this.AmountOfFuel / this.FuelTankSize) * 100) + "%\n");


        }
    }
}
