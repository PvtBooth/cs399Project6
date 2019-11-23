using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Mail;

namespace GAM150_Watcher
{
  public partial class DataForm : Form
  {
    public DataForm()
    {
      InitializeComponent();
    }

    private void button2_Click(object sender, EventArgs e)
    {
      MailMessage message = new MailMessage("digipenteamA001@Gmail.com", "digipenteamA001@Gmail.com");

      //Get the info fram the form
      message.Subject = "Game Team Email";
      StringBuilder sb = new StringBuilder();
      //sb.AppendLine("Text" + Environment.NewLine);
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
        NetworkCredential netCred = new NetworkCredential("digipenteamA001@Gmail.com", "norming&storming");
        smtpClient.Credentials = netCred;
        smtpClient.EnableSsl = true;
        
        Attachment logFileAttachment = new Attachment(Program.zipFile);
        message.Attachments.Add(logFileAttachment);
        smtpClient.Send(message);
      }
    }

    private void button1_Click(object sender, EventArgs e)
    {
      Close();
    }
  }
}
