using System;
using System.Data;
using System.Data.OleDb;

namespace ConsoleApplication6
{
    class Program
    {
        private OleDbConnection OleCon;
        private OleDbCommand OleCom;
        private OleDbDataReader OleDR;
        private string ConStr;

        static void Main(string[] args)
        {
            string Database = "mydb";
            Program prog = new Program();
            prog.Connect(Database);
            prog.ListTables(Database);
            prog.Disconnect();
        }

        private void Connect(string Database)
        {
            string Server = "MyPC\\SQLEXPRESS";
            string Username = "sa";
            string Password = "";

            ConStr = "Provider=SQLOLEDB;";   //Microsoft.Jet.OLEDB.4.0;";
            ConStr += "Data Source=" + Server + ";";
            ConStr += "Initial Catalog=" + Database + ";";
            ConStr += "User ID=" + Username + ";";
            ConStr += "Password=" + Password + ";";
            ConStr += "Integrated Security=SSPI;";

            OleCon = new OleDbConnection(ConStr);      //Data provider for MS ACCESS

			try
			{
				Console.WriteLine("Openning connection...\r\n");

				if (OleCon.State == ConnectionState.Closed)
				{
					OleCon.Open();
				}
				Console.WriteLine("OLE Connection State = " + OleCon.State + "\r\n");
			}
			catch (System.Data.OleDb.OleDbException Ex)
			{
				Console.WriteLine(Ex.Message + "\r\n");
				Console.WriteLine("Could not access the database.\r\nPlease make sure you completed the fields with the correct information and try again.\r\n\r\nMore details:\r\n" + Ex.Message);
			}
        }


        private void ListTables(string Database)
		{
			if (OleCon.State == ConnectionState.Open)
			{
                OleCom = new OleDbCommand();
                OleCom.Connection = OleCon;
//                OleCom.CommandType = CommandType.TableDirect;
                OleCom.CommandText = "SELECT * from [mydb].[dbo].[user]";
                OleDR = OleCom.ExecuteReader();

                Console.WriteLine("Content in Table [" + Database + "].[dbo].[user] :\r\n");
                while (OleDR.Read() == true)
                {
                    Console.WriteLine("Values >> " + OleDR[0] + " , " + OleDR[1] + " , " + OleDR[2] + " , " + OleDR[3] + " , " + OleDR[4] + "\r\n");
                }
			}
		}

		private void Disconnect()
		{
			if (OleCon.State == ConnectionState.Open)
			{
				OleCon.Close();
			}
		}
    }
}
