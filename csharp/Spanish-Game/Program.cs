using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Data.SqlClient;


namespace SpanishGame
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            FDialog inputDialog = new FDialog();
            //display the form as a dialog of the main form
            string userInput;
            bool b = false;

            do
            {
                inputDialog.resetInputText();
                inputDialog.ShowDialog();
                inputDialog.setIpTextFocus();
                userInput = inputDialog.getInputText();
                int val = 0;
                b = Int32.TryParse(userInput, out val);

                if (userInput != null && b)
                {
                    int dbqMax = getmaxQues();

                    if (dbqMax >= val)
                        Application.Run(new SpanishGame(dbqMax, val));
                    else
                    {
                        MessageBox.Show("Questions in the Database are less than the no of questions entered. Please enter the value below " + dbqMax.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        b = false;
                    }
                }
                else if (userInput == null)
                {
                    b = true;
                }
                else if (!b)
                {
                        MessageBox.Show("Invalid no of Questions! ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        b = false;
                }

            } while(!b);
        }

        static int getmaxQues()
        {
            string connectionString = GetConnectionString();
            string queryString = "select max(ROWS) from sysindexes where id = object_id('maintable');";
            SqlConnection connection = new SqlConnection(connectionString);
            int val = 0;

            try
            {
                connection.Open();
                SqlCommand command = connection.CreateCommand();
                command.CommandText = queryString;
                SqlDataReader reader = command.ExecuteReader();

                if (reader.Read())
                {
                    val = reader.GetInt32(0);
                }
                reader.Close();
                connection.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return val;
        }

        static string GetConnectionString()
        {
            // To avoid storing the connection string in your code, 
            // you can retrieve it from a configuration file.
            return "Data Source=OWNER-PC\\SQLEXPRESS;Initial Catalog=Daylan;"
                + "Integrated Security=true";
        }
    }
}
