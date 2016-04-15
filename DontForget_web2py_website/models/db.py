# -*- coding: utf-8 -*-

#########################################################################
## This scaffolding model makes your app work on Google App Engine too
## File is released under public domain and you can use without limitations
#########################################################################

## if SSL/HTTPS is properly configured and you want all HTTP requests to
## be redirected to HTTPS, uncomment the line below:
# request.requires_https()

## app configuration made easy. Look inside private/appconfig.ini
from gluon.contrib.appconfig import AppConfig
## once in production, remove reload=True to gain full speed
myconf = AppConfig(reload=True)


if not request.env.web2py_runtime_gae:
    ## if NOT running on Google App Engine use SQLite or other DB
    db = DAL(myconf.take('db.uri'), pool_size=myconf.take('db.pool_size', cast=int), check_reserved=['all'])
else:
    ## connect to Google BigTable (optional 'google:datastore://namespace')
    db = DAL('google:datastore+ndb')
    ## store sessions and tickets there
    session.connect(request, response, db=db)
    ## or store session in Memcache, Redis, etc.
    ## from gluon.contrib.memdb import MEMDB
    ## from google.appengine.api.memcache import Client
    ## session.connect(request, response, db = MEMDB(Client()))

## by default give a view/generic.extension to all actions from localhost
## none otherwise. a pattern can be 'controller/function.extension'
response.generic_patterns = ['*'] if request.is_local else []
## choose a style for forms
response.formstyle = myconf.take('forms.formstyle')  # or 'bootstrap3_stacked' or 'bootstrap2' or other
response.form_label_separator = myconf.take('forms.separator')


## (optional) optimize handling of static files
# response.optimize_css = 'concat,minify,inline'
# response.optimize_js = 'concat,minify,inline'
## (optional) static assets folder versioning
# response.static_version = '0.0.0'
#########################################################################
## Here is sample code if you need for
## - email capabilities
## - authentication (registration, login, logout, ... )
## - authorization (role based authorization)
## - services (xml, csv, json, xmlrpc, jsonrpc, amf, rss)
## - old style crud actions
## (more options discussed in gluon/tools.py)
#########################################################################

from gluon.tools import Auth, Service, PluginManager

auth = Auth(db)
service = Service()
plugins = PluginManager()

## configure email
mail = auth.settings.mailer
mail.settings.server = 'logging' if request.is_local else myconf.take('smtp.server')
mail.settings.sender = myconf.take('smtp.sender')
mail.settings.login = myconf.take('smtp.login')

## configure auth policy
auth.settings.registration_requires_verification = False
auth.settings.registration_requires_approval = False
auth.settings.reset_password_requires_verification = True

#########################################################################
## Define your tables below (or better in another model file) for example
##
## >>> db.define_table('mytable',Field('myfield','string'))
##
## Fields can be 'string','text','password','integer','double','boolean'
##       'date','time','datetime','blob','upload', 'reference TABLENAME'
## There is an implicit 'id integer autoincrement' field
## Consult manual for more options, validators, etc.
##
## More API examples for controllers:
##
## >>> db.mytable.insert(myfield='value')
## >>> rows=db(db.mytable.myfield=='value').select(db.mytable.ALL)
## >>> for row in rows: print row.id, row.myfield
#########################################################################

## after defining tables, uncomment below to enable auditing
# auth.enable_record_versioning(db)


db = DAL('sqlite://storage.sqlite')
#user authorization stuff
auth = Auth(db)

auth.settings.extra_fields['auth_user']= [
     Field('phone', requires = IS_MATCH('^\d{10}$', error_message = 
        'Please enter your 10 digit phone number') ) ]

auth.define_tables()
#crud = Crud(db)

db.define_table('alarm',
                Field('user_id', 'reference auth_user'),
                Field('phone_number'),
                Field('reminder_date', 'date'),
                Field('reminder_time'),
                Field('reminder_message'),
                Field('repeat', 'boolean', default = False,
                     readable = False, writable = False,
                     label = 'Repeat every day'),
                Field('sent', 'boolean', default = False,
                    readable = False, writable = False),
                Field('repeat_offset', 'integer', label = 'How many days between each reminder',
                     readable = True, writable = True),
                Field('repeat_amount', 'integer', label = 'How many times would you like the reminder to repeat',
                     readable = True, writable = True))

db.define_table('blackList',
                Field('phone_number'))

db.define_table('addressBook',
                Field('user', 'reference auth_user', default=auth.user_id, writable=False,readable=False),
                Field('contact'),
                Field('phone_number'))


#will require enter a time of the form HH:MM
db.alarm.reminder_time.requires = IS_TIME()

if auth.user:
    db.alarm.user_id.default = auth.user_id
    db.alarm.phone_number.default = auth.user.phone
    db.alarm.repeat.writable = True
    db.alarm.repeat.readable = True
    db.alarm.repeat_offset.writable = True
    db.alarm.repeat_offset.readable = True
    db.alarm.repeat_amount.writable = True
    db.alarm.repeat_amount.readable = True
    #db.alarm.carrier.default = auth.user.carrier

#     #if quick alarm flag set, display only the time
#     if session.alarmType == "quick":
#         print "quick alarm is true"    #debugging statement
#         #hiding all the fields, b/c already have the info
#         db.alarm.phone_number.readable =db.alarm.phone_number.writable= False
#        # db.alarm.reminder_message.writable = db.alarm.reminder_message.readable = False
# #        db.alarm.reminder_time_zone.writable = db.alarm.reminder_time_zone.readable = False

# Non-writable (hidden) fields
db.alarm.user_id.writable = False
db.alarm.user_id.readable = False


db.alarm.phone_number.requires = IS_MATCH('^\d{10}$', error_message=
    'Please enter your 10 digit phone number')
db.addressBook.phone_number.requires = IS_MATCH('^\d{10}$', error_message=
    'Please enter your 10 digit phone number')

# Validation
db.alarm.reminder_date.requires = IS_DATE(format=T('%Y-%m-%d'), error_message = "Must be YYYY-DD-MM")
