# Attempting to send mail at the correct times.
from gluon.tools import Mail
import imaplib
import email
import datetime
from gluon.tools import prettydate

from email.header import decode_header


import time
mail = Mail()

mail.settings.server = 'smtp.gmail.com:587'
mail.settings.sender = 'dontforgetyourevent@gmail.com'
mail.settings.login = 'dontforgetyourevent@gmail.com:web2pyucsc'

def phoneProviderList(phonenumber):

    listOfNumbers = []
    listOfNumbers.append(phonenumber + "@text.wireless.alltel.com") #Alltel
    listOfNumbers.append(phonenumber + "@text.att.net")             #AT&T
    listOfNumbers.append(phonenumber + "@sms.mycricket.com")        #Cricket
    listOfNumbers.append(phonenumber + "@messaging.sprintpcs.com")  #Sprint
    listOfNumbers.append(phonenumber + "@page.nextel.com")          #Nextel
    listOfNumbers.append(phonenumber + "@tmomail.net")              #T-Mobile
    listOfNumbers.append(phonenumber + "@email.uscc.net")           #U.S. Cellular
    listOfNumbers.append(phonenumber + "@vtext.com")                #Verizon
    return listOfNumbers

def checkMail():
    m = imaplib.IMAP4_SSL('imap.gmail.com')
    (retcode, capabilities) = m.login('dontforgetyourevent@gmail.com', 'web2pyucsc')
    m.list()
    m.select('inbox')

    m.select('inbox')
    typ, data = m.search(None, 'ALL')
    ids = data[0]
    id_list = ids.split()

    #get the most recent email id
    latest_email_id = int( id_list[-1] )

    imapdb = DAL("imap://dontforgetyourevent@gmail.com:web2pyucsc@smtp.gmail.com:993", pool_size=1)
    imapdb.define_tables()

    q = imapdb.INBOX.seen == False
    q &= imapdb.INBOX.created == request.now.date()
    q &= imapdb.INBOX.size < 6000
    unread = imapdb(q).count()

    rows = imapdb(q).select()

    mymessage = imapdb(imapdb.INBOX.uid == latest_email_id).select().first()

    stop1 = "Stop"
    stop2 = "stop"

    m.select(readonly=1)
    (retcode, messages) = m.search(None, '(UNSEEN)')
    if retcode == 'OK':
        for i in messages[0].split():
            typ, data = m.fetch( i, '(RFC822)' )
            for response_part in data:
                if isinstance(response_part, tuple):
                    msg = email.message_from_string(response_part[1])
                    varSubject = msg['subject']
                    varFrom = msg['from']
                    ms = str(msg)
                    first = '+'
                    if first in varFrom:
                        if stop1 in ms or stop2 in ms:
                            number = str(varFrom)[2:12]
                            rows = db((db.blackList.phone_number == number)).select()
                            if len(rows) == 0:
                                mail.send(to=[varFrom],
                                    subject='Your Reminder',
                                    message = 'Stopping all reminders')
                                typ, data = m.store(i,'+FLAGS','\\Seen')
                                number = str(varFrom)[2:12]
                                db.blackList.insert(phone_number = number)
                                db.commit()
                    elif varFrom[0:10].isdigit():
                        if stop1 in ms or stop2 in ms:
                            number = str(varFrom)[0:10]
                            rows = db((db.blackList.phone_number == number)).select()
                            if len(rows) == 0:
                                mail.send(to=[varFrom],
                                    subject='Your Reminder',
                                    message = 'Stopping all reminders')
                                typ, data = m.store(i,'+FLAGS','\\Seen')
                                number = str(varFrom)[0:10]
                                db.blackList.insert(phone_number = number)
                                db.commit()

while True: # inf loop

    # Current Date YYYY-MM-DD

    currentDate = datetime.datetime.now().date

    print(":re")

    # Current Time HH:MM:00


    currentTime = datetime.datetime.now().time()
    print(currentTime)

    rows = db((db.alarm.reminder_date == currentDate) & (db.alarm.reminder_time <= currentTime) & (db.alarm.sent == False)).select()

    for row in rows:

        theList = phoneProviderList(row.phone_number)
        print(row.phone_number + " -- " + str(row.reminder_date) + " -- " + str(row.reminder_time) + " -- " + row.reminder_message)

        blackListed = db((db.blackList.phone_number == row.phone_number)).select()
        if len(rows) == 0:
            mail.send(to=theList,subject="Don't Forget!",message=row.reminder_message)
            row.update_record(sent = True)
            db.commit()
            # FIXME

            if row.repeat and row.repeat_amount > 0:

                newDate = datetime.datetime.today() + datetime.timedelta(days=row.repeat_offset)
                newRepeatAmount = row.repeat_amount - 1 
                row.update_record(reminder_date  = newDate)
                row.update_record(repeat_amount = newRepeatAmount)
                row.update_record(sent = False)

                db.commit()
        else:
            print("Blacklisted!")

    checkMail()

    time.sleep(50) # check every 50s    