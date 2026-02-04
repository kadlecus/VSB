using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using Newtonsoft.Json;
using desktop_projekt.Model;
using System.Diagnostics;
using System.Net.Http.Json;
using System.Windows;

namespace desktop_projekt.Services
{
    public class ParkingLotService
    {
        private readonly HttpClient _httpClient;

        public ParkingLotService()
        {
            _httpClient = new HttpClient();
            _httpClient.BaseAddress = new Uri("https://localhost:7020/");
        }


        public async Task<List<ParkingLot>> GetParkingLotsAsync()
        {
            var response = await _httpClient.GetAsync("ApiParkingLot");
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }

            var jsonReseponse = await response.Content.ReadAsStringAsync();
           
           
            return JsonConvert.DeserializeObject<List<ParkingLot>>(jsonReseponse);


        }

        public async Task<List<ParkingSpot>> GetParkingSpotsAsync(int lotid)
        {
            var response = await _httpClient.GetAsync($"ApiParkingLot/Detail/{lotid}");
            response.EnsureSuccessStatusCode();

            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }
            
            var jsonresponse = await response.Content.ReadAsStringAsync();

            return JsonConvert.DeserializeObject<List<ParkingSpot>>(jsonresponse);


        }

        public async Task<List<ReservationRequest>> GetAllReservationsAsync()
        {
            var response = await _httpClient.GetAsync($"ApiParkingLot/GetReservations");
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            
            }
            
            var jsonresponse = await response.Content.ReadAsStringAsync();

            return JsonConvert.DeserializeObject<List<ReservationRequest>>(jsonresponse);
        }

        public async Task<List<ParkingSpotHistory?>> GetParkingSpotHistory(int spotid)
        {
            var response = await _httpClient.GetAsync($"ApiParkingLot/History/{spotid}");
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);

            }

            var jsonresponse = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<List<ParkingSpotHistory>>(jsonresponse);

        }

        public async Task<bool> DeleteReservationAsync(int id)
        {
            var response = await _httpClient.DeleteAsync($"ApiParkingLot/Delete/{id}");
            response.EnsureSuccessStatusCode();

            if (!response.IsSuccessStatusCode)
            {

                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }
            return response.IsSuccessStatusCode;
        }

        public async Task<bool> UpdateReservationAsync(ReservationRequest reservation)
        {
            var response = await _httpClient.PutAsJsonAsync($"ApiParkingLot/Update/", reservation);
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {

                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }
            return response.IsSuccessStatusCode;
        }


        public async Task<bool> UpdateManualStatusAsync(ParkingSpot spot)
        {
            var response = await _httpClient.PutAsJsonAsync("ApiParkingLot/UpdateDetail", spot);
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode) 
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }

            if(spot.Status == "Obsazené")
            {
                MessageBox.Show("Tohle místo je momenatálně obsazené");
                return false;
            }

            return response.IsSuccessStatusCode;
        }

        public async Task<bool> AddParkingSpot(int lotid)
        {
            var response = await _httpClient.PostAsync($"ApiParkingLot/AddSpot/{lotid}", null);
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }
            
            return response.IsSuccessStatusCode;
        }

        public async Task<bool> DeleteParkingSpotAsync(int spotId)
        {
            var response = await _httpClient.DeleteAsync($"ApiParkingLot/DeleteSpot/{spotId}");
            response.EnsureSuccessStatusCode();
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception("Chyba při volání API: " + response.ReasonPhrase);
            }


            return response.IsSuccessStatusCode;
        }
    }
}
