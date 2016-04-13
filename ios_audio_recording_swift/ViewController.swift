//
//  ViewController.swift
//  rants
//
//  Created by Gabe on 1/20/2016.
//  Copyright © 2015 Gabriel Velazquez. All rights reserved.
//

import UIKit

// This is the main View Controller that controls buttons allowing you to 
// navigate the rest of the app

class ViewController: UIViewController {

    //Xcode supplied function
    override func viewDidLoad() {
        self.view.backgroundColor = UIColor.grayColor()
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    //Xcode supplied function
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    //function to test opening up alert messages
    @IBAction func showMessage() {
        let alertController=UIAlertController(title: "Here",message: "Hello World" ,preferredStyle: UIAlertControllerStyle.Alert)
        alertController.addAction(UIAlertAction(title: "Press", style:
            UIAlertActionStyle.Default, handler:nil))
        self.presentViewController(alertController, animated: true, completion:nil)
    }
    
    //progamatically transition to a new view instead of using storyboard
    //opens a table view that displays recordings
    //storyboard viewcontroller identifier is in Main.storyboard
    @IBAction func recordView(sender: UIButton){
        let g = storyboard?.instantiateViewControllerWithIdentifier("viewrecz") as! viewRecs
         self.presentViewController(g, animated: true, completion: nil)
    }
    
    //progamatically transition to a new view instead of using storyboard
    //opens up a view controller to record/play
    //storyboard viewcontroller identifier is in Main.storyboard
    @IBAction func beginSession(sender: AnyObject){
        let vc = storyboard?.instantiateViewControllerWithIdentifier("recordingViewPage") as! recordingView
        //opens up the viewcontroller-keep
        self.presentViewController(vc, animated: false, completion:nil)
    }
    
    //opens up a view controller to page to test sql queries
    //storyboard viewcontroller identifier is in Main.storyboard
    @IBAction func viewRecordings(sender:AnyObject){
        let rec = storyboard?.instantiateViewControllerWithIdentifier("recList") as! recordings
        self.presentViewController(rec, animated:false, completion:nil)
    }
    
    
    //this connects to a button 
    //used to test actions and others items that are printed to the console for testing purposes
    @IBAction func checkTime(sender:AnyObject){
        let date = NSDate()
        let calendar = NSCalendar.currentCalendar()
        let components = calendar.components(NSCalendarUnit.Year.union(NSCalendarUnit.Minute), fromDate: date)
        let hour = components.hour
        let minutes = components.minute
        
        //print(hour)
        print (date)
        //var timestamp = NSDate()
        //var newTime = NSDate()
        let timestamp = NSDateFormatter.localizedStringFromDate(NSDate(), dateStyle: .MediumStyle, timeStyle: .ShortStyle)
        
        //printTimestamp()
        print(timestamp)
        
        
        let today = NSDate()
        let newTime = today.dateByAddingTimeInterval(-24*3600)

        print(newTime)
        
        
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss Z"
        let datex = dateFormatter.dateFromString(timestamp)!
        
        //if (datex == nil){
        //    print(":(")
        //}
        print(datex)
        
        
        /*if date.compare(datex!) == .OrderedAscending{
            print("old")
        } else {
            print("new")
        }*/
        
        //let strDate = "2015-11-01T00:00:00Z" // "2015-10-06T15:42:34Z"
        //let dateFormatter = NSDateFormatter()
        //dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ssZ"
        //print ( dateFormatter.dateFromString( strDate )! )

    }
  
}

