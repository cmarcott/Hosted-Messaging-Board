## Hosted Messaging Board

- This is a hosted service which allows users to create accounts, and post to different *messaging boards* depending on their interests

- It was created for a school project

# Breakdown

- Users can create and join *groups* in which they can view other user's posts in the groups they are a part of

- They can specify which group they would like to view in order to view new posts by other users

- They can leave or join new groups as they please

# Backend Breakdown

- Source Code written in C, C++, Python, PHP, HTML, and utilizes a mySQL database

- In order to create each webpage, a custom parser program breaks down .wpml configuration files and auto-generates the HTML code 

An example of a configuration file would be:

```
.o()
.g()
.f(varname="$uname",varvalue="$_POST['username']")
.f(varname="$sname",varvalue="$_POST['stream']")
.f(varname ="$contents",varvalue="mynl2br($_POST['postcontents'])")
.j(size="3",arg="Post to Stream ",arg="$sname")
.m(cmd="'./post '", "$_POST['username']","' '","'-un1qu3Ph@s3 '","$_POST['stream']","' -un1qu3Fl@g '","$contents"END)
.k()
.s(element="form",elementValue="display: inline")
.b(link="CallParser.php",name="Back to Post",hide="",name="wpmlfile",value="Post.wpml",hide="",name="filecreate",value="Post.php",hide="",name="username",value="$uname",hide="",name="stream",value="$sname")
.b(link="view.php",name="View Current Stream",hide="",name="username",value="$uname",hide="",name="stream",value="$sname",hide="",name="flag",value="st@rt")
.b(link="index.php",name="Home")
.w()
.c()
```

## Database Audit Program
The Database Audit program is named db.c, and once compiled will be named db

When used with the -clear flag (e.g. db -clear) the program
removes all of the posts, users, streams and any other information
from the tables in the database.
 
When used with the -reset flag the program deletes the tables
from the database.

When used with the -posts flag the program prints out all posts
stored in the database.

When used with the -users flag the program prints out all user names 
stored in the database. 
 
When used with the -streams flag the program prints out all stream
names stored in the database.
 
Order of the output does not matter for any of the options which print
the contents of a table.

When used with the -help flag it prints out a usage message.

