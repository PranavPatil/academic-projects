using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Linq;


namespace WindowsFormsApplication3
{
    public partial class Form1 : Form
    {
      public Form1()
      {
          InitializeComponent();
      }

      private void button1_Click(object sender, System.EventArgs e)
      {
          addToXml();          
      }
  
      private void button2_Click(object sender, EventArgs e)
      {
          readXml();
      }

      private void addToXml()
      {
          if (txtName.Text != null && txtTeam.Text != null && cmbPosition.SelectedItem != null)
          {
              XDocument xmlDoc = XDocument.Load("Players.xml");

              xmlDoc.Element("Players").Add(new XElement("Player", new XElement("Name", txtName.Text),
              new XElement("Team", txtTeam.Text), new XElement("Position", cmbPosition.SelectedItem.ToString())));

              xmlDoc.Save("Players.xml");
              readXml();
          }
          else
              MessageBox.Show("Please Enter all the Values",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
      }

      private void readXml()
      {
         XDocument xmlDoc = XDocument.Load("Players.xml");

         var players = from player in xmlDoc.Descendants("Player")
         select new
         {
            Name = player.Element("Name").Value,
            Team = player.Element("Team").Value,
            Position = player.Element("Position").Value,
         };

         txtResults.Text = "";
         foreach (var player in players)
         {
            txtResults.Text = txtResults.Text + "Name: " + player.Name + "\n";
            txtResults.Text = txtResults.Text + "Team: " + player.Team + "\n";
            txtResults.Text = txtResults.Text + "Position: " + player.Position + "\n\n";
         }

         if (txtResults.Text == "")
            txtResults.Text = "No Results.";
      }
    }
}

///http://www.csharp-station.com/tutorials/lesson13.aspx
// Another example with LINQ for Arrays
/*
class LINQQueryExpressions
{
    static void Main()
    {

        // Specify the data source.
        int[] scores = new int[] { 97, 92, 81, 60 };

        // Define the query expression.
        IEnumerable<int> scoreQuery =
            from score in scores
            where score > 80
            select score;

        // Execute the query.
        foreach (int i in scoreQuery)
        {
            Console.Write(i + " ");
        }            
    }
}
// Output: 97 92 81
*/