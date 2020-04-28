namespace SpanishGame
{
    partial class SpanishGame
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.AnswerBox = new System.Windows.Forms.TextBox();
            this.next_btn = new System.Windows.Forms.Button();
            this.replaybtn = new System.Windows.Forms.Button();
            this.titlelbl = new System.Windows.Forms.Label();
            this.acentbtn = new System.Windows.Forms.Button();
            this.notxt = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // AnswerBox
            // 
            this.AnswerBox.Location = new System.Drawing.Point(79, 155);
            this.AnswerBox.Multiline = true;
            this.AnswerBox.Name = "AnswerBox";
            this.AnswerBox.Size = new System.Drawing.Size(498, 202);
            this.AnswerBox.TabIndex = 0;
            this.AnswerBox.TextChanged += new System.EventHandler(this.AnswerBox_TextChanged);
            // 
            // next_btn
            // 
            this.next_btn.Location = new System.Drawing.Point(79, 363);
            this.next_btn.Name = "next_btn";
            this.next_btn.Size = new System.Drawing.Size(498, 51);
            this.next_btn.TabIndex = 1;
            this.next_btn.Text = "next";
            this.next_btn.UseVisualStyleBackColor = true;
            this.next_btn.Click += new System.EventHandler(this.next_btn_Click);
            // 
            // replaybtn
            // 
            this.replaybtn.Location = new System.Drawing.Point(79, 90);
            this.replaybtn.Name = "replaybtn";
            this.replaybtn.Size = new System.Drawing.Size(498, 48);
            this.replaybtn.TabIndex = 2;
            this.replaybtn.Text = "replay";
            this.replaybtn.UseVisualStyleBackColor = true;
            this.replaybtn.Click += new System.EventHandler(this.button1_Click);
            // 
            // titlelbl
            // 
            this.titlelbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.titlelbl.ForeColor = System.Drawing.Color.Red;
            this.titlelbl.Location = new System.Drawing.Point(214, 28);
            this.titlelbl.Name = "titlelbl";
            this.titlelbl.Size = new System.Drawing.Size(320, 35);
            this.titlelbl.TabIndex = 3;
            this.titlelbl.Text = "Caza en Español";
            // 
            // acentbtn
            // 
            this.acentbtn.Location = new System.Drawing.Point(595, 94);
            this.acentbtn.Name = "acentbtn";
            this.acentbtn.Size = new System.Drawing.Size(88, 23);
            this.acentbtn.TabIndex = 4;
            this.acentbtn.Text = "Accent Mark";
            this.acentbtn.UseVisualStyleBackColor = true;
            this.acentbtn.Click += new System.EventHandler(this.acentbtn_Click);
            // 
            // notxt
            // 
            this.notxt.Location = new System.Drawing.Point(642, 130);
            this.notxt.Name = "notxt";
            this.notxt.ReadOnly = true;
            this.notxt.Size = new System.Drawing.Size(41, 20);
            this.notxt.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(592, 133);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "QuesNo:";
            // 
            // SpanishGame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(709, 444);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.notxt);
            this.Controls.Add(this.acentbtn);
            this.Controls.Add(this.titlelbl);
            this.Controls.Add(this.replaybtn);
            this.Controls.Add(this.next_btn);
            this.Controls.Add(this.AnswerBox);
            this.Name = "SpanishGame";
            this.Text = "SpanishGame";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox AnswerBox;
        private System.Windows.Forms.Button next_btn;
        private System.Windows.Forms.Button replaybtn;
        private System.Windows.Forms.Label titlelbl;
        private System.Windows.Forms.Button acentbtn;
        private System.Windows.Forms.TextBox notxt;
        private System.Windows.Forms.Label label1;
    }
}

