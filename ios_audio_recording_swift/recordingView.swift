//
//  recordingView.swift
//  rants
//
//  Created by Gabe on 1/20/16.
//  Copyright © 2016 Gabriel Velazquez. All rights reserved.
//

import UIKit
import MapKit
import AVFoundation


class recordingView: UIViewController, AVAudioPlayerDelegate, AVAudioRecorderDelegate {

    
    @IBOutlet weak var playButton:UIButton!
    @IBOutlet weak var stopButton:UIButton!
    @IBOutlet weak var recordButton:UIButton!
    var recordingSession: AVAudioSession!
    var audioRecorder:AVAudioRecorder?
    var audioPlayer:AVAudioPlayer!
    var k = NSString()
    var s = NSURL()
    
    @IBOutlet weak var reccname: UITextField!
    @IBOutlet weak var setUpButton: UIButton!
    @IBOutlet weak var dbfield: UITextField!
    @IBOutlet weak var loadit: UITextField!

   
    override func viewDidLoad() {
        super.viewDidLoad()
        
        stopButton.enabled=false
        playButton.enabled=false
    
    }
    

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    @IBAction func setUp(sender: AnyObject){
       
        //appends .caf to input fulename
        //fixed retreiving the file from another view
        var x = reccname.text! + ".caf"
        //print(x)
        //creates a directory URL to save the recording
        var documents: AnyObject = NSSearchPathForDirectoriesInDomains( NSSearchPathDirectory.DocumentDirectory,  NSSearchPathDomainMask.UserDomainMask, true)[0]
        var str =  documents.stringByAppendingPathComponent(x)
        var audioFileURL = NSURL.fileURLWithPath(str as String)
        
        //print(str)
        
        //Creates an audio session to play and record audio
        let audioSession = AVAudioSession.sharedInstance()
        do{
            try audioSession.setCategory(AVAudioSessionCategoryPlayAndRecord)
        }catch{
            print(error)
        }
        
        //sets up audio file attributes
        do{
            try audioSession.setCategory(AVAudioSessionCategoryPlayAndRecord, withOptions: AVAudioSessionCategoryOptions.DefaultToSpeaker)
            
            let recorderSetting:[String: AnyObject] = [
                AVFormatIDKey:Int(kAudioFormatAppleIMA4), //Int required in Swift2
                AVSampleRateKey:44100.0,
                AVNumberOfChannelsKey:2,
                AVEncoderBitRateKey:12800,
                AVLinearPCMBitDepthKey:16,
                AVEncoderAudioQualityKey:AVAudioQuality.Max.rawValue
            ]
            
            audioRecorder = try AVAudioRecorder(URL:audioFileURL, settings:recorderSetting)
            audioRecorder?.delegate=self
            audioRecorder?.meteringEnabled = true
            audioRecorder?.prepareToRecord()
            //print(audioRecorder!.url)
        }
        catch{
            print(error)
        }
        
        
        let date = NSDate()
        let calendar = NSCalendar.currentCalendar()
        let components = calendar.components(NSCalendarUnit.Year.union(NSCalendarUnit.Minute), fromDate: date)
        let hour = components.hour
        let minutes = components.minute
        
        print (hour)

        let documents2 = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let url1 = documents2.URLByAppendingPathComponent("contacts.db")
        
        let contactDB = FMDatabase(path: url1.path)
        
        //testing converting filetypes from one to another
        //and using 'as TYPE' cast to extract certain data types
        k = url1.path!
        s = NSURL.fileURLWithPath(k as String)
        //print(s)
        //print(k)
        
        
        //print(audioFileURL.path!)
        //print(audioFileURL)
        var dd = NSDate()
        //print(dd.)
        let timestamp = NSDateFormatter.localizedStringFromDate(NSDate(), dateStyle: .MediumStyle, timeStyle: .ShortStyle)
        
        if contactDB.open() {
            let insertSQL = "INSERT INTO test (recordingk, fileurl, fdate) VALUES ('\(reccname.text!)', '\(audioFileURL.path!)','\(timestamp)')"
            
            let result = contactDB.executeUpdate(insertSQL,
                withArgumentsInArray: nil)
            
            if !result {
                loadit.text = "Failed to add contact"
                print("here")
                print("Error: \(contactDB.lastErrorMessage())")
            } else {
                loadit.text = "Contact Added"
                print("worked?")
                //name.text = ""
                //comment.text = ""
            }
        } else {
            print("stopp")
            print("Error: \(contactDB.lastErrorMessage())")
        }
        
        
    }
    
    //initiates recording audio
    @IBAction func record(sender: AnyObject){
        if let player = audioPlayer{
            if player.playing{
                player.stop()
              //  playButton.setImage(UIImage(named: "play"), forState: UIControlState.Normal)
                playButton.selected = false
            }
        }
        if let recorder = audioRecorder {
            if !recorder.recording{
                let audioSession = AVAudioSession.sharedInstance()
                do{
                    try audioSession.setActive(true)
                    recorder.record()
                   // recordButton.setImage(UIImage(named:"recording"), forState: UIControlState.Selected)
                    recordButton.selected = true
                    print("record starting?")
                } catch {
                    print(error)
                }
            }
            else{
                recorder.pause()
               // recordButton.setImage(UIImage(named:"pause"), forState:UIControlState.Normal)
                recordButton.selected=false
                print("recording paused")
            }
        }
        print("record pressed")
        stopButton.enabled=true
        playButton.enabled=true
    }
    
  
    //stops recording or playing
    @IBAction func stop(sender: AnyObject){
        //recordButton.setImage(UIImage(named:"record"), forState: UIControlState.Normal)
        recordButton.selected = false
        //playButton.setImage(UIImage(named: "play"), forState: UIControlState.Normal)
        playButton.selected = false
        
        stopButton.enabled = false
        playButton.enabled = true
        
        let audioSession = AVAudioSession.sharedInstance()
        
        do{
            try audioSession.setActive(false)
        } catch {
            print(error)
        }
        
        print("stop pressed")

    }
    
    //plays an item
    @IBAction func play(sender: AnyObject){
         print(audioRecorder!.url)
        
        if let recorder = audioRecorder {
            do {
                try audioPlayer = AVAudioPlayer(contentsOfURL: (recorder.url))
                audioPlayer.prepareToPlay()
                audioPlayer.play()
                print("play works")
            } catch {
                //audioPlayer=nil
                print(error)            }
           // }
        }
        print("play pressed")
    }
        

    //changes background color to green
    @IBAction func green(sender: UIButton!){
                    self.view.backgroundColor = UIColor.greenColor()
    }
    
    //transitions to initial view controller
    @IBAction func stopit(sender: UIButton!){
            //self.view.backgroundColor=UIColor.grayColor()
            let recordIt = self.storyboard!.instantiateViewControllerWithIdentifier("r1") as UIViewController
            //let vc = storyboard?.instantiateViewControllerWithIdentifier("recordingViewPage") as! recordingView
            
            //let recordIt:UIViewController=ViewController()
            self.presentViewController(recordIt, animated: false, completion:nil)
    }
    
    
    
    
    //test selecting data from contact database
    @IBAction func findContact(sender: AnyObject) {
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let url1 = documents.URLByAppendingPathComponent("contacts.db")
        
        let contactDB = FMDatabase(path: url1.path)
        
        if contactDB.open() {
            let querySQL = "SELECT recname FROM test WHERE name = '\(loadit.text)'"
            
            let results:FMResultSet? = contactDB.executeQuery(querySQL,
                withArgumentsInArray: nil)
            
            if results?.next() == true {
                reccname.text = results?.stringForColumn("recname")
                print(results?.stringForColumn("recname"))
                dbfield.text = "Record Found"
            } else {
                dbfield.text = "Record not found"
                reccname.text = ""
            }
            contactDB.close()
        } else {
            print("Error: \(contactDB.lastErrorMessage())")
        }
    }
    
    
    
    
    func audioRecorderDidFinishRecording(recorder: AVAudioRecorder, successfully
        flag: Bool) {
        if flag {
        let alertMessage = UIAlertController(title: "Finish Recording",
        message: "Successfully recorded something!", preferredStyle: .Alert)
        alertMessage.addAction(UIAlertAction(title: "OK", style: .Default,
        handler: nil))
        presentViewController(alertMessage, animated: true, completion: nil)
        } }


}  
