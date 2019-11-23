using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GAM150_Watcher
{
  public partial class FormPreAlpha : Form
  {
    public FormPreAlpha()
    {
      InitializeComponent();
    }

    private void SubmitButton_Click(object sender, EventArgs e)
    {
      MailMessage message = new MailMessage("digipenteamA001@Gmail.com", "digipenteamA001@Gmail.com");

      //Get the info fram the form
      message.Subject = "Feedback: Alpha";
      StringBuilder sb = new StringBuilder();
      sb.AppendLine("#####NAMES:" + Environment.NewLine);
      sb.AppendLine("Sound," +    (Name_1_Rate_1.Checked) + "," + (Name_1_Rate_2.Checked) + "," + (Name_1_Rate_3.Checked) + Environment.NewLine);
      sb.AppendLine("Graphics," + (Name_2_Rate_1.Checked) + "," + (Name_2_Rate_2.Checked) + "," + (Name_2_Rate_3.Checked) + Environment.NewLine);
      sb.AppendLine("Controls," + (Name_3_Rate_1.Checked) + "," + (Name_3_Rate_2.Checked) + "," + (Name_3_Rate_3.Checked) + Environment.NewLine);
      sb.AppendLine("Gameplay," + (Name_4_Rate_1.Checked) + "," + (Name_4_Rate_2.Checked) + "," + (Name_4_Rate_3.Checked) + Environment.NewLine);
      sb.AppendLine("Menus," +    (Name_5_Rate_1.Checked) + "," + (Name_5_Rate_2.Checked) + "," + (Name_5_Rate_3.Checked) + Environment.NewLine);
      sb.AppendLine("#####THOUGHTS:" + Environment.NewLine);
      sb.AppendLine(richTextBox2.Text + Environment.NewLine);
      sb.AppendLine(richTextBox1.Text + Environment.NewLine);
      message.Body = sb.ToString();
      message.IsBodyHtml = false;

      //Close the form
      Close();

      //Zip up the log file
      ZipArchive archive = ZipFile.Open(Program.zipFile, ZipArchiveMode.Create);
      archive.CreateEntryFromFile(Program.logFile, Path.GetFileName(Program.logFile), CompressionLevel.Optimal);
      archive.Dispose();

      //Send the email
      SendEmail(message);
    }

    private void SendEmail(MailMessage message)
    {
      using (SmtpClient smtpClient = new SmtpClient())
      {
        smtpClient.Host = "smtp.gmail.com";
        smtpClient.UseDefaultCredentials = false;
        NetworkCredential netCred = new NetworkCredential("digipenteamA001@Gmail.com", "ourmemesaredreams");
        smtpClient.Credentials = netCred;
        smtpClient.EnableSsl = true;

        Attachment logFileAttachment = new Attachment(Program.zipFile);
        message.Attachments.Add(logFileAttachment);
        smtpClient.Send(message);
      }
    }

    private void CloseButton_Click(object sender, EventArgs e)
    {
      Close();
    }

    private void radioButton2_CheckedChanged(object sender, EventArgs e)
    {

    }

    private void Name_3_Meh_CheckedChanged(object sender, EventArgs e)
    {

    }

    private void Name_1_Exellent_CheckedChanged(object sender, EventArgs e)
    {

    }

    private void Name_4_Exellent_CheckedChanged(object sender, EventArgs e)
    {

    }

    private void label1_Click(object sender, EventArgs e)
    {

    }

    private void label4_Click(object sender, EventArgs e)
    {

    }

    private void textBox1_TextChanged(object sender, EventArgs e)
    {

    }

    private void Name_Custom_1_TextChanged(object sender, EventArgs e)
    {

    }

    private void textBox3_TextChanged(object sender, EventArgs e)
    {

    }

    private void label5_Click(object sender, EventArgs e)
    {

    }

    private void richTextBox2_TextChanged(object sender, EventArgs e)
    {

    }

    private void FormPreAlpha_Load(object sender, EventArgs e)
    {

    }
  }
}
