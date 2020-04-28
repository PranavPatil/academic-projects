using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Runtime.InteropServices; // DllImport()

namespace SpanishGame
{
    public partial class SpanishGame : Form
    {
        [DllImport("WinMM.dll")]

        public static extern bool PlaySound(string fname, int Mod, int flag);

        //  flag values for SoundFlags  argument on PlaySound
        public int SND_SYNC = 0x0000;  // play synchronously (default) 
        public int SND_ASYNC = 0x0001;  // play asynchronously 
        public int SND_NODEFAULT = 0x0002;  // silence (!default) if sound not found 
        public int SND_MEMORY = 0x0004;  // pszSound points to a memory file 
        public int SND_LOOP = 0x0008;  // loop the sound until next sndPlaySound 
        public int SND_NOSTOP = 0x0010;  // don't stop any currently playing sound 

        public int SND_NOWAIT = 0x00002000; // don't wait if the driver is busy 
        public int SND_ALIAS = 0x00010000; // name is a registry alias 
        public int SND_ALIAS_ID = 0x00110000; // alias is a predefined ID 
        public int SND_FILENAME = 0x00020000; // name is file name 
        public int SND_RESOURCE = 0x00040004; // name is resource name or atom 
        public int SND_PURGE = 0x0040;  // purge non-static events for task 
        public int SND_APPLICATION = 0x0080;  // look for application specific association 
        SqlDataReader reader;
        SqlConnection connection;
        string connectionString;
        string queryString;
        SqlCommand command;

        private int correct = 0;
        private string currplayfile = null;
        private string currentans = null;
        private HashSet<int> qlist;
        private int maxques;
        private int maxdbques;
        private int counter;


        public SpanishGame(int dbqMax, int maxq)
        {            
                InitializeComponent();
                initDb();
                this.maxques = maxq;
                this.maxdbques = dbqMax;
                qlist = new HashSet<int>();
                counter = 0;
                MessageBox.Show("Listo para iniciar el juego español!", "Vayamos", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                currentans = read();
                counter++;
                next_btn.Enabled = false;
                notxt.Text = counter.ToString();
        }

        private void initDb()
        {
            connectionString = GetConnectionString();
            connection = new SqlConnection(connectionString);         
            try
            {
                connection.Open();
                command = connection.CreateCommand();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private void uninitDb()
        {
            try
            {
                connection.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private int random(int maxvalue)
        {
          DateTime x = DateTime.Now;
          Random r = new Random((x.Minute * x.Second) + x.Hour);
          int qno = 0;
          bool isadded = true;

          while(isadded) {
            qno = r.Next(maxvalue);
            isadded = qlist.Contains(qno);            
          }

          return qno;
        }

        private string read()
        {
            string text = null;
            int quesid = -1;

            try
            {
                quesid = random(maxdbques) + 1;
                queryString = "SELECT id, text, filename FROM dbo.maintable where id = " + 
                               quesid + ";";

                command.CommandText = queryString;
                reader = command.ExecuteReader();

                if (reader.Read())
                {
                    //int id = reader.GetInt32(0);
                    text = reader.GetString(1);
                    //MessageBox.Show("Ques Text : " + text, text, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    //MessageBox.Show(text, text, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    currplayfile = reader.GetString(2);

                    //System.Console.WriteLine("\t{0}\t{1}\t{2}", reader[0], reader[1], reader[2]);
                    PlaySound("C:/Users/owner/c sharp projects/SoundTest/SpanishGame/SpanishGame/" + currplayfile, 0, SND_ASYNC | SND_FILENAME);
                    qlist.Add(quesid-1);
                }

                reader.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return text;
         }
   
        static private string GetConnectionString()
        {
            // To avoid storing the connection string in your code, 
            // you can retrieve it from a configuration file.
            return "Data Source=OWNER-PC\\SQLEXPRESS;Initial Catalog=Daylan;"
                + "Integrated Security=true";
        }

        private void next_btn_Click(object sender, EventArgs e)
        {
            bool loop = false;

            if (counter <= maxques && currentans != null)
            {
                if (AnswerBox.Text != null && !AnswerBox.Text.Equals(""))
                {
                    if (System.String.Compare(AnswerBox.Text, currentans, true) == 0)
                    {
                        MessageBox.Show("Respuesta Correcta", "Bueno", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        correct++;
                    }
                    else
                    {
                        DialogResult result =  MessageBox.Show("Respuesta Incorrecta" + System.Environment.NewLine + "Otra Prueba ?"
                                        , "Ooops", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);

                        if (result == DialogResult.Yes)
                        {
                            loop = true;
                            if (currplayfile != null && !currplayfile.Equals(""))
                                PlaySound("C:/Users/owner/c sharp projects/SoundTest/SpanishGame/SpanishGame/" + currplayfile, 0, SND_ASYNC | SND_FILENAME);

                        }
                        else if (result == DialogResult.No)
                        {
                            loop = false;
                            MessageBox.Show("Respuesta Incorrecta" + System.Environment.NewLine + "Solución: " + currentans
                                            , "Ooops", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        }
                    }

                    AnswerBox.Text = "";
                }
            }
            else
            {
                MessageBox.Show("Total Correct Ans: " + correct.ToString(), "Su puntuación", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                Application.Exit(); 
            }

            next_btn.Enabled = false;

            if (!loop && counter < maxques)
            {
                currentans = read();
                counter++;
                notxt.Text = counter.ToString();
            }
            else if (!loop && counter == maxques) 
            {
                counter++;
            }

            if (counter > maxques)
            {
                MessageBox.Show("Total Correct Ans: " + correct.ToString(), "Su puntuación", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                Application.Exit(); 
            }
        }

        private void AnswerBox_TextChanged(object sender, EventArgs e)
        {
            if (AnswerBox.Text != null && !AnswerBox.Text.Equals(""))
                next_btn.Enabled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
           if (currplayfile != null && !currplayfile.Equals(""))
             PlaySound("C:/Users/owner/c sharp projects/SoundTest/SpanishGame/SpanishGame/" + currplayfile, 0, SND_ASYNC | SND_FILENAME);
        }

        private void acentbtn_Click(object sender, EventArgs e)
        {
            if (AnswerBox.Text != null && !AnswerBox.Text.Equals(""))
            {
               if (AnswerBox.Text.EndsWith("A"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length-1) + "Á";
               else if (AnswerBox.Text.EndsWith("a"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "á";
               else if (AnswerBox.Text.EndsWith("E"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "É";
               else if (AnswerBox.Text.EndsWith("e"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "é";
               else if (AnswerBox.Text.EndsWith("I"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "Í";
               else if (AnswerBox.Text.EndsWith("i"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "í";
               else if (AnswerBox.Text.EndsWith("O"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "Ó";
               else if (AnswerBox.Text.EndsWith("o"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "ó";
               else if (AnswerBox.Text.EndsWith("U"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "Ú";
               else if (AnswerBox.Text.EndsWith("u"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "ú"; 
               else if (AnswerBox.Text.EndsWith("N"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "Ñ";
               else if (AnswerBox.Text.EndsWith("n"))
                  AnswerBox.Text = AnswerBox.Text.Substring(0, AnswerBox.Text.Length - 1) + "ñ";
            }
        }
    }
}

