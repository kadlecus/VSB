using System;
using System.Globalization;
using static MyApp.Program;
using static System.Net.Mime.MediaTypeNames;


[Flags]
public enum WeightUnit { 

    g = 0,
    dkg = 1,
    kg = 2

}

namespace MyApp
{
    internal class Program
    {
        public struct Weight
        {
            public double weightValue { get; set; }
            public WeightUnit Unit { get; set; }

            public double GetNormalizedWeight()
            {
                if (this.Unit == WeightUnit.g)
                {
                    this.weightValue = this.weightValue / 1000;
                }
                else if (this.Unit == WeightUnit.dkg)
                {
                    this.weightValue = this.weightValue / 100;
                }
                else if (this.Unit == WeightUnit.kg) 
                {
                    this.weightValue = this.weightValue;
                }
                return this.weightValue;
            }

        }
        public class Product {

            public string? Name { get; set; }
            public double priceUnitOne { get; set; }
            public int? unitCount { get; set; }
            public Weight Unit { get; set; }

        }


        public static Weight ParseWeight(string weightLine)
        {
            weightLine = weightLine.Substring(7);
            weightLine = weightLine.Trim();

            double unit;

            string[] splitWeight = weightLine.Split(' ');

            Weight weight = new Weight();

            if (double.TryParse(splitWeight[0], CultureInfo.InvariantCulture, out unit))
            {

                weight.weightValue = unit;

            }
            else
            {
                Console.WriteLine("Error");
            }

            if (splitWeight[1] == "g")
            {
                weight.Unit = WeightUnit.g;
            }
            else if (splitWeight[1] == "dkg")
            {
                weight.Unit = WeightUnit.dkg;
            }
            else if (splitWeight[1] == "kg") {
                weight.Unit = WeightUnit.kg;
            }


            return weight;
        }

        public static Product[] ParseData(string data) {

            int itemCount = 0;

            data.Trim();

            string[] newLines = data.Split('\n');


            for (int i = 1; i < newLines.Length; i++)
            {
                newLines[i] = newLines[i].Trim();

                if (newLines[i].Length > 2) {
                    newLines[i] = newLines[i].Substring(2);
                }

                if (!newLines[i].StartsWith("weight:") && !newLines[i].StartsWith("price:") && !newLines[i].StartsWith("quantity:") && !string.IsNullOrEmpty(newLines[i]))
                {
                    itemCount++;
                }

            }

            Product[] products = new Product[itemCount + 1];

            for (int i = 0; i < products.Length; i++)
            {
                products[i] = new Product();
            }


            int productIndex = 0;
            double numberPrice;
            int numberCount;

            for (int i = 1; i < newLines.Length; i++)
            {

                if (newLines[i].StartsWith("weight:"))
                {

                    products[productIndex].Unit = ParseWeight(newLines[i]);

                }
                else if (newLines[i].StartsWith("price:"))
                {
                    newLines[i] = newLines[i].Substring(6);
                    newLines[i] = newLines[i].Trim();

                    if(double.TryParse(newLines[i], CultureInfo.InvariantCulture, out numberPrice)) 
                    {
                        products[productIndex].priceUnitOne = numberPrice;
                    }

                }
                else if (newLines[i].StartsWith("quantity:"))
                {
                    newLines[i] = newLines[i].Substring(9);
                    newLines[i] = newLines[i].Trim();

                    if (int.TryParse(newLines[i], CultureInfo.InvariantCulture, out numberCount))
                    {
                        products[productIndex].unitCount = numberCount;
                    }

                }
                else
                {
                    if (i != 1)
                    {
                        productIndex++;
                    }
                    newLines[i] = newLines[i].Trim();
                    products[productIndex].Name = newLines[i];
                    
                }

            }

            return products;
        }

        public static double GetTotalProductsPrice(Product[] products)
        {
            double sum = 0;

            for (int i = 0; i < products.Length; i++)
            {
                if (products[i].unitCount.HasValue)
                {

                    sum += products[i].priceUnitOne * products[i].unitCount.GetValueOrDefault();
                }
            }
            return sum;
        }

        public static double GetAverageItemWeight(Product[] products)
        {
            double totalUnitCount = 0;
            double average = 0;
            double newItemCount = 0;
            for (int i = 0; i < products.Length; i++)
            {  
                totalUnitCount += products[i].Unit.GetNormalizedWeight();
                newItemCount++;
            }
            
            average = totalUnitCount / (newItemCount - 1);

            return Math.Round(average, 3); ;
        }

        static void Main(string[] args)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("cs-CZ");
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            string data = File.ReadAllText("data.txt");
           
            Product[] products = ParseData(data);

            Console.WriteLine("Produkty:");
            for (int i = 0; i < products.Length - 1; i++)
            {
                if (products[i].unitCount == null)
                {
                    Console.WriteLine(products[i].Name + " " + "neznámé množství; " + products[i].priceUnitOne + " Kč" );
                }
                else {
                Console.WriteLine(products[i].Name + " " + products[i].unitCount + " ks; " + products[i].priceUnitOne + " Kč");
                
                }
            }
            Console.Write("\n");
            double sum = GetTotalProductsPrice(products);
            double averageWeight = GetAverageItemWeight(products);

            Console.WriteLine("Celková cena produktů: " + sum + " Kč");
            Console.WriteLine("Průměrná váha položky: " + averageWeight + " kg");
        }
    }
}