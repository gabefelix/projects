//
//  recordings.swift
//  rants
//
//  Created by Gabe on 2/8/16.
//  Copyright © 2016 Gabriel Velazquez. All rights reserved.
//

import UIKit


//this pages handles the database administration, creates/modifies two databases, 
// db test is used throughout the app, db recordings is used for testing purposes... 

class recordings: UIViewController {

   
    @IBOutlet weak var name: UITextField!
    @IBOutlet weak var comment: UITextField!
    @IBOutlet weak var status: UILabel!
    
    @IBOutlet weak var playtext: UITextField!
    
    //var urlString: NSURL = NSURL()
    var fileURL = NSString()
    var urlString = NSString()
    
    //variables to store url paths to access the database in different functions
    var z = NSURL()
    var f = NSString()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let filemgr = NSFileManager.defaultManager()
        
        //creates the directory url for storing the database
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let fileURL = documents.URLByAppendingPathComponent("contacts.db")
        
        f = fileURL.path!
        z = NSURL.fileURLWithPath(f as String!)
        
        print("here")
        
        //print(fileURL)
       // print("line")
        //print(fileURL.path)
       // print("line")
        //..print(urlString)
        //print(fileURL.path! as String)
        
       /* do{
        try filemgr.copyItemAtPath(fileURL.path!, toPath: newPath)
        } catch{
            print(error)
        }*/
        

       // !filemgr.fileExistsAtPath(urlString as String) {
            let db = FMDatabase(path: fileURL.path)
        if !db
            .open() {
            print("Unable to open database")
            return
        }
        
        do {
            //initial table is used to test sql commands with text fields
            try db.executeUpdate("CREATE TABLE IF NOT EXISTS test (ID INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, recname TEXT, fdate datetime default current_timestamp )", values: nil)
            
            try db.executeUpdate("CREATE TABLE IF NOT EXISTS time (datex NSDATE)", values:nil)
           
            //these two columns store the names of the recordings, and the filepaths of the recordings
            try db.executeUpdate("ALTER TABLE test ADD COLUMN recordingk", values: nil)
            //save filepaths
            //get timestamp
            try db.executeUpdate("ALTER TABLE test ADD COLUMN fileurl", values: nil)
            try db.executeUpdate("ALTER TABLE test ADD COLUMN fdate datetime default current_timestamp", values:nil)
        }catch let error as NSError {
            print("failed: \(error.localizedDescription)")
        }
        db.close()
    }
    
    //drop table 'test'
    //connected to a button on the view
    @IBAction func drop(sender: AnyObject){
        let db = FMDatabase(path: z.path)
        
        if db.open(){
            do{
                try db.executeUpdate("DROP TABLE test", values:nil)
            }catch let error as NSError{
            print("Drop Tables Failed, Error: \(error.localizedDescription)")
            }
        }
        db.close()
    }
    
    
    //saves the name and comment into database
    //first table/columns created, used to test insert statements
    @IBAction func saveData(sender: AnyObject) {
        
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let fileURL = documents.URLByAppendingPathComponent("contacts.db")
        let db = FMDatabase(path: fileURL.path)
        
        if db.open() {

            let insertSQL = "INSERT INTO test (name, recname) VALUES ('\(name.text)', '\(comment.text)')"
            
            let result = db.executeUpdate(insertSQL,
                withArgumentsInArray: nil)
            
            if !result {
                status.text = "Failed to add recording"
                print("Insert Failed")
                print("Error: \(db.lastErrorMessage())")
            } else {
                status.text = "Recording Added"
                print("Insert Successful")
                name.text = ""
                comment.text = ""
            }
        } else {
            print("Save Data, Error: \(db.lastErrorMessage())")
        }
    }
    
    
    //function that tests selecting data from sql database with data 
    //from a  form field from the view
    @IBAction func findContact(sender: AnyObject) {
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let fileURL = documents.URLByAppendingPathComponent("contacts.db")

        let db = FMDatabase(path: fileURL.path)
        
        if !db.open(){
            print("unable to open database")
            return
        }
        
        if db.open() {
            let querySQL = "SELECT recname FROM test WHERE name = '\(name.text)'"
            
            let results:FMResultSet? = db.executeQuery(querySQL,
                withArgumentsInArray: nil)
            
            if results?.next() == true {
                comment.text = results?.stringForColumn("recname")
                print(results?.stringForColumn("recname"))
                status.text = "Record Found"
            } else {
                status.text = "Record not found"
                comment.text = ""
            }
            db.close()
        } else {
            print("Error: \(db.lastErrorMessage())")
        }
    }
    
    
    
    
    //tests selecting from a different database created in an other view.
    @IBAction func playitem (sender: AnyObject) {
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let playURL = documents.URLByAppendingPathComponent("recordings.db")
        
        let db = FMDatabase(path: playURL.path)
        print(playURL.path)
        let x =  NSString()
        
        if !db.open(){
            print("unable to open database")
            return
        }
        if db.open() {
            let querySQL = "SELECT urlname FROM recordings WHERE (urlname) = '\(playtext.text)'"
            
            let aga:FMResultSet? = db.executeQuery(querySQL,
                withArgumentsInArray: nil)
            
            if aga?.next() == true {
                status.text = "Record Found"
                comment.text = aga?.stringForColumn("title")
                print(aga?.stringForColumn("title"))
                
                print("FINALLY YEA?")
                //x = (aga?.stringForColumn("urlname"))!
            } else {
                status.text = "Record not found"
                comment.text = ""
                print("Error: \(db.lastErrorMessage())")
            }
            db.close()
            
        } else {
            print("Error: \(db.lastErrorMessage())")
        }
        
        print(x as String)
    }
    
    
    //creates a button to transition to the main app view 
    @IBAction func back(sender: UIButton!){
        //self.view.backgroundColor=UIColor.grayColor()
        let recordIt = self.storyboard!.instantiateViewControllerWithIdentifier("r1") as UIViewController
        //let vc = storyboard?.instantiateViewControllerWithIdentifier("recordingViewPage") as! recordingView
        
        //let recordIt:UIViewController=ViewController()
        self.presentViewController(recordIt, animated: false, completion:nil)
    }
    
    
}
