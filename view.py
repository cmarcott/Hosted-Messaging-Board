#!/usr/bin/python3
import os
import sys
import string
import subprocess

def FindStreamName(post):
	name = post.split("\n")[0]
	return name

def FindSPostsRead(streamname, tstreamsorig, treadorig):
	count=0
	for stream in tstreamsorig:
		if stream == streamname:
			sPostsRead = treadorig[count] + 1
			break
		count += 1

	return sPostsRead

def SaveReadPostsNum(author, streamname, postsRead):
	postsReadstr = str(postsRead)
	subprocess.call(["./db","-updateR3ad",author,streamname,postsReadstr])

def SaveAllReadPostsNum(author, tstreams):

	for i in tstreams:
		count = 0
		streamsString = subprocess.check_output(["./db","-vi3w",i])
		streamsString = streamsString.decode("utf-8")
		posts = streamsString.split("-3ND")
		for post in posts:
			count+=1
		numPosts = count-1
		if numPosts<0:
			numPosts=0
		SaveReadPostsNum(author, i, numPosts)

def PrintAllStreams(incStreamUFiles, author, argv, decision):
	currentPost = 0
	posts=[]
	streamsname = ""
	for i in incStreamUFiles:
		streamsname = streamsname + i
		streamsname = streamsname + " "
	
	streamsString = subprocess.check_output(["./db","-allStr3ams",streamsname])
	streamsString = streamsString.decode("utf-8")
	posts = posts+streamsString.split("-3ND")

	streamsString = subprocess.check_output(["./db","-allP0stsR3ad",author])
	streamsString = streamsString.decode("utf-8")
	allPostsReadTemp = streamsString.split("\n")
	del allPostsReadTemp[len(allPostsReadTemp)-1]

	tstreams = []
	tread = []
	for i in allPostsReadTemp:
		tstreams.append(i.split(" ")[0])
		tread.append(i.split(" ")[1])

	count = 0
	for i in tread:
		tread[count] = int(i)
		count+=1

	movetoread = False

	readposts = []
	unreadposts = []
	tstreamsorig = tstreams
	treadorig = tread[:]
	count = 0
	for i in posts:
		line = i.split("\n")[0]
		for streamo in tstreams:
			if streamo == line:
				if tread[count] != 0:
					tread[count] -= 1
					movetoread = True
				else:
					movetoread = False
				if movetoread == True:
					readposts.append(i)
					break
				else:
					unreadposts.append(i)
					break
			count += 1

		count = 0
	posts = readposts + unreadposts
	postsRead = len(readposts)

	numPosts = len(posts)
	argc = len(argv)
	currentPost = postsRead

	nametoggle = argv[argc-1]

	if nametoggle == 'st@rt':
		# If the user clicked "Check for new posts, or if they are just starting"
		if decision == 'st@rt':
			if postsRead == numPosts:
				print ("\n----There are no more posts to show---- ")
				print(currentPost)
				exit()
			if currentPost == postsRead:
				if postsRead < numPosts:
					postsRead += 1
					streamname = FindStreamName(posts[currentPost])
					sPostsRead = FindSPostsRead(streamname, tstreamsorig, treadorig)
					SaveReadPostsNum(author, streamname, sPostsRead)
					print("Stream: ",end="")
					print(posts[currentPost])
					print(currentPost)
					exit()
				else:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
		elif decision == '-0ffS3t':
			offset = int(argv[argc-3])
			lastPost = int(argv[argc-2])
			currentPost = lastPost + offset 
			if currentPost >= numPosts:
				currentPost = numPosts
			if currentPost < 0:
				print("** This is the first post, there are no previous posts **\n")
				currentPost = 0
			if currentPost < 0 or currentPost > numPosts:
				print("\n----There are no more posts to show---- ")
				print(currentPost)
				exit()
			if currentPost >= numPosts:
				print ("\n----There are no more posts to show---- ")
				print(currentPost)
				exit()
			print("Stream: ",end="")
			print(posts[currentPost])
			if currentPost == postsRead:
				if postsRead < numPosts:
					postsRead += 1
					streamname = FindStreamName(posts[currentPost])
					sPostsRead = FindSPostsRead(streamname, tstreamsorig, treadorig)
					SaveReadPostsNum(author, streamname, sPostsRead)
					print(currentPost)
					exit()
			print(currentPost)
			exit()
		elif decision == '-m@ll':
			postsRead = numPosts
			#SaveReadPostsNum(author, streamname, postsRead)
			SaveAllReadPostsNum(author, tstreams)
			print ("\n----There are no more posts to show---- ")
			currentPost = postsRead
			print(currentPost)
			exit()
	elif nametoggle == 'nam3sort':
		while nametoggle != '-m@ll':
			currentPost = 0
			streamsString = subprocess.check_output(["./db","-anam3s0rt",streamsname])
			streamsString = streamsString.decode("utf-8")
			posts = streamsString.split("-3ND")

			# If the user clicked "Check for new posts, or if they are just starting"
			if decision == 'nam3sort':
				if numPosts == 0:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				print("Stream: ",end="")
				print(posts[currentPost])
				print(currentPost)
				exit()
			# If the user either clicked next post or previous post
			elif decision == '-0ffS3t':
				offset = int(argv[argc-3])
				lastPost = int(argv[argc-2])
				currentPost = lastPost + offset 
				if currentPost >= numPosts:
					currentPost = numPosts
				if currentPost < 0:
					print("** This is the first post, there are no previous posts **\n")
					currentPost = 0
				if currentPost < 0 or currentPost > numPosts:
					print("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				if currentPost >= numPosts:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				print("Stream: ",end="")
				print(posts[currentPost])
				print(currentPost)
				exit()
			elif decision == '-m@ll':
				postsRead = numPosts
				#SaveReadPostsNum(author, streamname, postsRead)
				SaveAllReadPostsNum(author, tstreams)
				currentPost = 0
				decision = 'nam3sort'


	print(currentPost)
	exit()


if __name__ == "__main__":
	#Main Program
	userFiles, incStreamUFiles, incStreamFiles, dataArray, tempContents, contentsLinebyLine = [], [], [], [], [], []

	number = len(sys.argv)
	if len(sys.argv) < 3 | ((sys.argv[number-1] == 'un1qu3Ph@s3') & (len(sys.argv) < 3)):
		print ("You need to specify an author... Please Try Again")
		print('0')
		exit()

	author = sys.argv[1]
	author = author + ' '
	for i in range(2,number):
		if i != (len(sys.argv)):
			if sys.argv[i] == 'un1qu3Ph@s3':
				startOtherArgs = i
				break
			author = author+sys.argv[i]
			author = author+' '
	author = author + ' '

	decision = ""
	streamname = ""
	start = False
	for i in sys.argv:
		if i == '-fl@g':
			break
		if start == True:
			streamname = streamname + i
		if i == 'un1qu3Ph@s3':
			start = True

	if streamname == 'all':
		#placeholder
		decision = ""
		start = False
		for i in sys.argv:
			if start == True:
				decision = i
				break
			if i == '-fl@g':
				start = True
		argv = sys.argv
		streamsString = subprocess.check_output(["./db","-ch00seStr3am",author])
		streamsString = streamsString.decode("utf-8")
		incStreamUFiles = streamsString.split()
		PrintAllStreams(incStreamUFiles, author, argv, decision)

		print("Shouldnt be here")
		print('0')
		exit()
	else:
		#The user has picked an individual stream
		streamsString = subprocess.check_output(["./db","-vi3w",streamname])
		streamsString = streamsString.decode("utf-8")
		posts = streamsString.split("-3ND")

		postsReadstr = subprocess.check_output(["./db","-postsRead",author,streamname])
		postsReadstr = postsReadstr.decode("utf-8")
		postsRead = int(postsReadstr)

		numPosts = len(posts)-1
		argc = len(sys.argv)
		currentPost = postsRead

		nametoggle = sys.argv[argc-1]

		#placeholder
		decision = ""
		start = False
		for i in sys.argv:
			if start == True:
				decision = i
				break
			if i == '-fl@g':
				start = True

		if nametoggle == 'st@rt':
			# If the user clicked "Check for new posts, or if they are just starting"
			if decision == 'st@rt':
				if postsRead == numPosts:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				if numPosts == 0:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				if currentPost == postsRead:
					if postsRead < numPosts:
						postsRead += 1
						SaveReadPostsNum(author, streamname, postsRead)
						print(posts[currentPost])
						print(currentPost)
						exit()
					else:
						print ("\n----There are no more posts to show---- ")
						print(currentPost)
						exit()
			elif decision == '-0ffS3t':
				offset = int(sys.argv[argc-3])
				lastPost = int(sys.argv[argc-2])
				currentPost = lastPost + offset 
				if currentPost >= numPosts:
					currentPost = numPosts
				if currentPost < 0:
					print("** This is the first post, there are no previous posts **\n")
					currentPost = 0
				if currentPost < 0 or currentPost > numPosts:
					print("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				if currentPost >= numPosts:
					print ("\n----There are no more posts to show---- ")
					print(currentPost)
					exit()
				print(posts[currentPost])
				if currentPost == postsRead:
					if postsRead < numPosts:
						postsRead += 1
						SaveReadPostsNum(author, streamname, postsRead)
						print(currentPost)
						exit()
				print(currentPost)
				exit()
			elif decision == '-m@ll':
				postsRead = numPosts
				SaveReadPostsNum(author, streamname, postsRead)
				print ("\n----There are no more posts to show---- ")
				currentPost = postsRead
				print(currentPost)
				exit()
		elif nametoggle == 'nam3sort':
			while nametoggle != '-m@ll':
				currentPost = 0
				streamsString = subprocess.check_output(["./db","-inam3s0rt",streamname])
				streamsString = streamsString.decode("utf-8")
				posts = streamsString.split("-3ND")

				# If the user clicked "Check for new posts, or if they are just starting"
				if decision == 'nam3sort':
					if numPosts == 0:
						print ("\n----There are no more posts to show---- ")
						print(currentPost)
						exit()
					print(posts[currentPost])
					print(currentPost)
					exit()
				# If the user either clicked next post or previous post
				elif decision == '-0ffS3t':
					offset = int(sys.argv[argc-3])
					lastPost = int(sys.argv[argc-2])
					currentPost = lastPost + offset 
					if currentPost >= numPosts:
						currentPost = numPosts
					if currentPost < 0:
						print("** This is the first post, there are no previous posts **\n")
						currentPost = 0
					if currentPost < 0 or currentPost > numPosts:
						print("\n----There are no more posts to show---- ")
						print(currentPost)
						exit()
					if currentPost >= numPosts:
						print ("\n----There are no more posts to show---- ")
						print(currentPost)
						exit()
					print(posts[currentPost])
					print(currentPost)
					exit()
				elif decision == '-m@ll':
					postsRead = numPosts
					SaveReadPostsNum(author, streamname, postsRead)
					currentPost = 0
					#print(currentPost)
					decision = 'nam3sort'
						

			print(currentPost)
			exit()



