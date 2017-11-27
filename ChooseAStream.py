#!/usr/bin/python3
import os
import sys
import string
import subprocess

if __name__ == "__main__":
	#Main Program
	userFiles, incStreamUFiles, incStreamFiles, dataArray, tempContents, contentsLinebyLine = [], [], [], [], [], []

	number = len(sys.argv)
	if len(sys.argv) < 2:
		print ("You need to specify an author... Please Try Again")
		exit()

	author = sys.argv[1]
	author = author + ' '
	for i in range(2,number):
		if i != (len(sys.argv)):
			if sys.argv[i] == 'un1qu3Ph@s3':
				break
			author = author+sys.argv[i]
			author = author+' '
	author = author + ' '

	streamsString = subprocess.check_output(["./db","-ch00seStr3am",author])
	streamsString = streamsString.decode("utf-8")
	incStreamUFiles = streamsString.split()

	#Checks if the user is a part of any streams
	if not incStreamUFiles:
		print ("The user is not included in any streams... Please Try Again")
		exit()

	print ("<form method=\"Post\" action=\"view.php\">")
	thecount = 0
	for i in incStreamUFiles:
		if thecount == 0:
			print ("<input type=\"radio\" name=\"stream\" value=\"" + i + "\" checked/>" + i + "<br>")
		else:
			print ("<input type=\"radio\" name=\"stream\" value=\"" + i + "\" />" + i + "<br>")
		thecount += 1
		#print (i, end = '')

	exit();