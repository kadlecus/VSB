namespace IGasolineEngineDatabase
{
    public interface IGasolineEngine
    { 
        public double AmountOfFuel { get; }
        public double FuelTankSize {  get; }
             
        public void Refuel(double quantity);
    
    }
}
