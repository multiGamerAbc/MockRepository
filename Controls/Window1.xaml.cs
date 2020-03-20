///////////////////////////////////////////////////////////////////////
// Window2.xaml.cs - Window with text controls                       //
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApplication3
{
  /// <summary>
  /// Interaction logic for Window1.xaml
  /// </summary>
  public partial class Window1 : Window
  {
    public Window1()
    {
      InitializeComponent();
      Title = "WPF Controls";
    }

    private void button1_Click(object sender, RoutedEventArgs e)
    {
      if (Para2.Inlines.Count > 0)
      {
        Para2.Inlines.Clear();
        Text1.Text = "Now is the hour for all good men to come to the aid of their country.";
      }
      else
      {
        Para2.Inlines.Add(new Run(
          "Call me Ishmael. Some years ago- never mind how long precisely- having little or no money in my purse, " +
          "and nothing particular to interest me on shore, I thought I would sail about a little and see the " +
          "watery part of the world. It is a way I have of driving off the spleen and regulating the circulation. " +
          "Whenever I find myself growing grim about the mouth; whenever it is a damp, drizzly November in my soul; " +
          "whenever I find myself involuntarily pausing before coffin warehouses, and bringing up the rear of every " +
          "funeral I meet; and especially whenever my hypos get such an upper hand of me, that it requires a strong " +
          "moral principle to prevent me from deliberately stepping into the street, and methodically knocking " +
          "people's hats off- then, I account it high time to get to sea as soon as I can. This is my substitute for " +
          "pistol and ball. With a philosophical flourish Cato throws himself upon his sword; I quietly take to the ship. " +
          "There is nothing surprising in this. If they but knew it, almost all men in their degree, some time or other, " +
          "cherish very nearly the same feelings towards the ocean with me. "
        ));
        Text1.Text = "Not everything that can be counted counts, and not everything that counts can be counted.";
      }
    }

    private void button2_Click(object sender, RoutedEventArgs e)
    {
      Window2 win = new Window2();
      win.Title = "More WPF Controls";
      win.Show();
    }
  }
}
