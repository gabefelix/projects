//
//  viewRecs.swift
//  rants
//
//  Created by Gabe on 2/21/16.
//  Copyright © 2016 Gabriel Velazquez. All rights reserved.
//

import UIKit
import AVFoundation

class viewRecs: UIViewController, UITableViewDataSource, UITableViewDelegate, AVAudioPlayerDelegate {
    
    //will store the names of the recordings
    var arrayNames:[String] = []
    // stores the filepaths of the recordings
    var arrayFiles: [String] = []
    
    //stores the recording date of creation
    var arrayDates: [String] = []
    var rowindex = 0
    
    var recordingSession: AVAudioSession!
    //var audioRecorder:AVAudioRecorder?
    var audioPlayer:AVAudioPlayer!
    var looper = false
    
    @IBOutlet var thetable: UITableView!
    override func viewDidLoad() {
        super.viewDidLoad()
        
        thetable.dataSource = self
        thetable.delegate = self
        
        let documents = try! NSFileManager.defaultManager().URLForDirectory(.DocumentDirectory, inDomain: .UserDomainMask, appropriateForURL: nil, create: false)
        let fileURL = documents.URLByAppendingPathComponent("contacts.db")

        let db = FMDatabase(path: fileURL.path)
        
        if db.open() {
            let querySQL = "SELECT recordingk FROM test"
            let results_lab_test:FMResultSet? = db.executeQuery(querySQL, withArgumentsInArray: nil)
            while results_lab_test?.next() == true {
                if let resultString = results_lab_test?.stringForColumn("recordingk"){
                    arrayNames.append(resultString)
                }
            }
            let querySQL2 = "SELECT fileurl FROM test"
            let files:FMResultSet? = db.executeQuery(querySQL2, withArgumentsInArray: nil)
            while files?.next() == true {
                if let resultString2 = files?.stringForColumn("fileurl"){
                    arrayFiles.append(resultString2)
                }
            }
            let querySQL3 = "SELECT fdate FROM test"
            let dates:FMResultSet? = db.executeQuery(querySQL3, withArgumentsInArray: nil)
            while dates?.next() == true{
                if let resultsString3 = files?.stringForColumn("FDATE"){
                    arrayDates.append(resultsString3)
                }
            }
            let l = arrayDates.count
            print(arrayDates)
            //print(arrayDates[l-1])
            db.close()
        }
    }
    

    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) ->
        Int {
        // Return the number of rows in the section.
        return arrayNames.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath:
        NSIndexPath) -> UITableViewCell {
        let cellIdentifier = "Cell"
        let cell = tableView.dequeueReusableCellWithIdentifier(cellIdentifier,
        forIndexPath: indexPath)
        // Configure the cell...
        cell.textLabel?.text = arrayNames[indexPath.row]
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath){

        let cellIdentifier = "Cell"
        let cell = tableView.dequeueReusableCellWithIdentifier(cellIdentifier,
            forIndexPath: indexPath)
        cell.textLabel!.text = "playing:" + arrayNames[indexPath.row]
        cell.tag = indexPath.row
        rowindex = indexPath.row
        
        /*******************************/
        //play the item in the table
        /*******************************/
        var audioFileURL = NSURL.fileURLWithPath(arrayFiles[indexPath.row] as String)
        print(audioFileURL)
        
        self.audioPlayer?.delegate = nil
        self.audioPlayer?.stop()
        //let fileURL = NSURL(fileURLWithPath: path)
        guard let p = try? AVAudioPlayer(contentsOfURL: audioFileURL) else {return} // nicer
        self.audioPlayer = p
        // omitted error checking
        self.audioPlayer.prepareToPlay()
        self.audioPlayer.delegate = self
        if self.looper {
            self.audioPlayer.numberOfLoops = -1
        }
        self.audioPlayer.play()
    }
    
    func playfile(sender:UITableViewCell) -> Void{
        print(arrayFiles[rowindex])
    }

}
    
