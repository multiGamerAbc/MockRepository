///////////////////////////////////////////////////////////////////////
// Window2.xaml.cs - Second Window with more controls                //
//                                                                   //
// Jim Fawcett, CSE681 - Software Modeling and Analysis, Summer 2009 //
///////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
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

namespace WpfApplication3
{
  /// <summary>
  /// Interaction logic for Window2.xaml
  /// </summary>
  public partial class Window2 : Window
  {
    public Window2()
    {
      InitializeComponent();
    }

    private void progressBar1_ValueChanged(
      object sender, RoutedPropertyChangedEventArgs<double> e
    )
    {
      //MessageBox.Show("made progress"); 
    }

    private void button1_Click(object sender, RoutedEventArgs e)
    {
      if (textBlock1.Text.Length == 0)
        textBlock1.Text = "Been clicked!";
      else
        textBlock1.Text = "";
    }
  }
}
