///////////////////////////////////////////////////////////////////////
// Window2.xaml.cs - Second Window with more controls                //
//                                                                   //
// Jim Fawcett, CSE681 - Software Modeling and Analysis, Summer 2009 //
///////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Data;



using System.Threading.Tasks;

using System.Windows.Navigation;

using System.Threading;
using System.IO;
//using MsgPassingCommunication;

namespace WpfApplication3
{
    /// <summary>
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window3 : Window
    {
        //private Translater translater;

        public Window3()
        {
            InitializeComponent();
            ArrayList a = new ArrayList();
            a.Add("OK");
            a.Add("GOOD");
            for(int i = 0; i < a.Count; i++)
            {
                listbox5a.Items.Add(a[i]);
            }
            listbox5a.Items.Add("ANY");
  

        }

        private void progressBar1_ValueChanged(
          object sender, RoutedPropertyChangedEventArgs<double> e
        )
        {
            //MessageBox.Show("made progress"); 
        }

        private ArrayList LoadListBoxData()

        {
            ArrayList itemsList = new ArrayList();
            itemsList.Add("Coffie");
            itemsList.Add("Tea");
            itemsList.Add("Orange Juice");
            itemsList.Add("Milk");
            itemsList.Add("Mango Shake");
            itemsList.Add("Iced Tea");
            itemsList.Add("Soda");
            itemsList.Add("Water");    
            return itemsList;
        }

        private void listbox5a_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            String str = listbox5a.SelectedItem.ToString();
            //String str2 = str.Substring(str.IndexOf(':') + 2);
            textBox5.Text += (textBox5.Text.Length > 0 ? "\n" + str : str);

            for (int i = 0; i < listbox5a.Items.Count; i++)
            {
                String str3 = (listbox5a.Items[i].ToString());
                //String str4 = str3.Substring(str.IndexOf(':') + 2);
                textBox5.Text += (textBox5.Text.Length > 0 ? "\n" + str3 : str3);
            }

            listbox5a.Items.Add("YUP");

        }

        private void ListItem_Click(object sender, RoutedEventArgs e)
        {
            textBox1.Text = textBox1.Text + "LIST ITEM ";

        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            item13.Content = "abc";
        }

        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            textBox1.Text = textBox1.Text + "OK";
            //    if (textBlock1.Text.Length == 0)
            //        textBlock1.Text = "Been clicked!";
            //    else
            //        textBlock1.Text = "";
            //}
        }
    }
}
