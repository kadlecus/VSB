using Accessibility;
using desktop_projekt.Services;
using desktop_projekt.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace desktop_projekt
{
    /// <summary>
    /// Interakční logika pro EditReservations.xaml
    /// </summary>
    public partial class EditReservations : Window
    {
        private readonly ParkingLotService _parkingLotService;
        public EditReservations()
        {
            InitializeComponent();
            _parkingLotService = new ParkingLotService();
        }

        private async void ShowReservations_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<ReservationRequest> list = await _parkingLotService.GetAllReservationsAsync();
                EditReservationsGrid.ItemsSource = list;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button) sender;

            if (EditReservationsGrid.SelectedItem is ReservationRequest request)
            {
                UpdateReservation update = new UpdateReservation(request);
                update.ShowDialog();
            }
            
        }

        private async void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button) sender;
            if (button.Tag is int id)
            {
                if (await _parkingLotService.DeleteReservationAsync(id))
                {
                    MessageBox.Show($"Povedlo se odstranit rezervaci s Id: {id}");
                    
                }
                else
                {
                    MessageBox.Show("Něco se pokazilo");
                }
            
            }
        
        }
    }
}
