#!/usr/bin/python3
import os
import sys
import string

#Function to set all streams a user is a part of to Read
def SetAllFilesRead(incStreamFiles, author, StreamsDatasList):

	numStreams = 0
	for stream in incStreamFiles:
		postsRead = len(StreamsDatasList[numStreams])
		postsRead -= 1
		SaveReadPostsNum(author, stream, postsRead)
		numStreams += 1

#Function to find the stream name of a post for printing
def FindStreamName(incStreamFiles, StreamsList, AllTuples, dateline):

	savedPost = ""
	for tuple in AllTuples:
		if dateline[ :-1] == tuple[0]:
			savedPost = tuple[1]
			break
	count=0
	if savedPost:
		for stream in StreamsList:
			if savedPost in stream:
				break
			count+=1
	return incStreamFiles[count]

#Function to Save the new number of read posts to a User File
def SaveReadPostsNum(author, olddecision, postsRead):
	contents = []
	userFileName = "./messages/" + olddecision + "StreamUsers"
	userFile = open(userFileName, "r")
	contents = userFile.readlines()

	appendString = ""
	postsReadString = str(postsRead)
	postsReadLength = len(postsReadString)
	iteration = 5-postsReadLength
	for i in range (iteration):
		appendString = appendString + " "
	appendString = appendString+postsReadString
	finalLine = ""

	for line in contents:
		if author in line:
			finalLine = line[ :70]
			oldNum = line[70: ]
			finalLine = finalLine + appendString + "\n"
			break
	userFile.close()
	if postsRead > int(oldNum):
		userFile = open(userFileName, "w")
		for line in contents:
			if author in line:
				userFile.write(finalLine)
			else:
				userFile.write(line)
		userFile.close()

#Marks a new post read to the correct stream when browsing All streams
def AddReadToCorrectFile(author, incStreamFiles, StreamsList, NumberRead, UnreadTuples, dateline, AddedDates):

	#Does not allow a post to be marked read twice
	savedPost = ""
	if AddedDates:
		for date in AddedDates:
			if dateline[ :-1] == date:
				return AddedDates

	for tuple in UnreadTuples:
		if dateline[ :-1] == tuple[0]:
			AddedDates.append(tuple[0])
			savedPost = tuple[1]
			break
	count=0
	if savedPost:
		for stream in StreamsList:
			if savedPost in stream:
				break
			count+=1
	else:
		return AddedDates
	NumberRead[count] += 1
	SaveReadPostsNum(author, incStreamFiles[count], NumberRead[count])
	return AddedDates

#Function that sorts a stream by name
def SortByName(olddecision, contents, contentsLinebyLine, dataArray, inAll, incStreamFiles, StreamsList, AllTuples):
	allusers, users, usersFull, Posts, sortedPosts, finalLines = [], [], [], [], [], []
	byte = 0
	for line in contentsLinebyLine:
		if line.startswith("Sender: "):
			end = len(line)
			allusers.append(line[8:end-1])
	allusers.sort()
	#adds the first user to the list
	users.append(allusers[0])
	count = 0
	#removes duplicates
	for user in allusers:
		count += 1
		exists = False
		for i in users:
			if user == i:
				exists = True
			else:
				continue
		if exists == False:
			users.append(allusers[count-1])
	#appends full Sender line to be searched
	for i in users:
		usersFull.append("Sender: " + i + "\n")
	dCount = 0
	for d in dataArray:
		dCount += 1
	dCount += 6

	for d in range(dCount):
		Posts.append("")

	count=0
	#Sort lines into posts
	for line in contentsLinebyLine:
		if line.startswith("Sender: "):
			count += 1
		Posts[count-1] = Posts[count-1] + line

	#Sort posts
	for user in usersFull:
		for post in Posts:
			if post.startswith(user):
				sortedPosts.append(post)

	#Initialize FinalLines List
	for line in contentsLinebyLine:
		finalLines.append("")

	line=0
	#Convert posts back to lines
	for post in sortedPosts:
		for c in post:
			if c == '\n':
				finalLines[line] = finalLines[line]+c
				line += 1
				continue
			else:
				finalLines[line] = finalLines[line]+c

	return finalLines;

#A function to print all the streams in a list sorted by date
def PrintAllStreams(incStreamFiles, incStreamUFiles, author, argv, decision):
	StreamsList, StreamsDatasList, StreamsUsersList, NumberRead, StreamsPostList, UnreadStreams, ReadStreams, UnreadLinebyLine = [],[],[],[],[],[],[],[]
	ReadLineByLine, UnreadPosts, ReadPosts, tempDateFinder, UDatesArray, RDatesArray, UnReadList, AllUnreadPosts, AllReadPosts = [],[],[],[],[],[],[],[],[]
	AllUDates, AllRDates, SortedUnread, SortedRead, AllPostsSorted, AllPostLinesSorted, TempAllPostLinesSorted = [],[],[],[],[],[],[]
	AllStreams, AddedDates = [],[]
	count = 0
	#Get information from all necessary files storing them into arrays
	for file in incStreamFiles:
		fileName = "./messages/" + file + "Stream"
		fileDataName = "./messages/" + file + "StreamData"
		fileUsersName = "./messages/" + file + "StreamUsers"
		with open(fileName, "r") as f:
			StreamsList.append(f.read())
			f.close
		with open(fileDataName, "r") as f:
			a = "0\n"
			StreamsDatasList.append(f.readlines())
			StreamsDatasList[count] = [a] + StreamsDatasList[count]
			f.close()
		with open(fileUsersName,"r") as f:
			StreamsUsersList.append(f.readlines())
			f.close()
		count+=1
	#Finds how many posts have been read from each stream
	for lst in StreamsUsersList:
		for i in lst:
			slicedLine = ""
			if i.startswith(author):
				slicedLine = i[66: ]
				NumberRead.append(int(slicedLine))

	#Reads in all Posts
	count=0
	byte=0
	for lst in NumberRead:
		byte = 0
		maxLen = len(StreamsList[count])
		AllStreams.append("")
		while byte < maxLen:
			AllStreams[count] = AllStreams[count] + StreamsList[count][byte]
			byte += 1
		count += 1


	#Splits unread posts from read posts
	count=0
	byte=0
	for lst in NumberRead:
		postsRead = int(lst)
		byte = int(StreamsDatasList[count][postsRead])
		maxLen = len(StreamsList[count])
		UnreadStreams.append("")
		while byte < maxLen:
			UnreadStreams[count] = UnreadStreams[count] + StreamsList[count][byte]
			byte += 1
		count += 1

	count=0
	byte=0

	#Splits read posts from unread posts
	for num in NumberRead:
		postsRead = int(num)
		stopByte = int(StreamsDatasList[count][postsRead])
		byte = 0
		ReadStreams.append("")
		while byte < stopByte:
			ReadStreams[count] = ReadStreams[count] + StreamsList[count][byte]
			byte += 1
		count += 1

	count = 0
	AllLineByLine=[]
	#Sort All into Lines
	for stream in AllStreams:
		AllLineByLine.append("")
		count2 = 0
		AllLineByLine[count] = []
		AllLineByLine[count].append("")
		for c in stream:
			AllLineByLine[count][count2] = AllLineByLine[count][count2] + c
			if c == '\n':
				AllLineByLine[count].append("")
				count2+=1
		AllLineByLine[count].pop()
		count+=1

	count = 0
	#Sort Unread into Lines
	for stream in UnreadStreams:
		UnreadLinebyLine.append("")
		count2 = 0
		UnreadLinebyLine[count] = []
		UnreadLinebyLine[count].append("")
		for c in stream:
			UnreadLinebyLine[count][count2] = UnreadLinebyLine[count][count2] + c
			if c == '\n':
				UnreadLinebyLine[count].append("")
				count2+=1
		UnreadLinebyLine[count].pop()
		count+=1

	count = 0
	#Sort Read into Lines
	for stream in ReadStreams:
		ReadLineByLine.append("")
		count2 = 0
		ReadLineByLine[count] = []
		ReadLineByLine[count].append("")
		for c in stream:
			ReadLineByLine[count][count2] = ReadLineByLine[count][count2] + c
			if c == '\n':
				ReadLineByLine[count].append("")
				count2+=1
		ReadLineByLine[count].pop()
		count+=1

	count=0
	AllPosts = []
	#Sort All into Posts:
	for stream in AllLineByLine:
		count2=0
		AllPosts.append("")
		AllPosts[count] = []
		AllPosts[count].append("")
		started = False
		for line in AllLineByLine[count]:
			if line.startswith("Sender: ") and started == True:
				count2 += 1
				AllPosts[count].append("")
			AllPosts[count][count2] = AllPosts[count][count2] + line
			started = True
		count+=1

	count=0
	#Sort Unread into Posts:
	for stream in UnreadLinebyLine:
		count2=0
		UnreadPosts.append("")
		UnreadPosts[count] = []
		UnreadPosts[count].append("")
		started = False
		for line in UnreadLinebyLine[count]:
			if line.startswith("Sender: ") and started == True:
				count2 += 1
				UnreadPosts[count].append("")
			UnreadPosts[count][count2] = UnreadPosts[count][count2] + line
			started = True
		count+=1

	count=0
	#Sort Read into Posts:
	for stream in ReadLineByLine:
		count2=0
		ReadPosts.append("")
		ReadPosts[count] = []
		ReadPosts[count].append("")
		started = False
		for line in ReadLineByLine[count]:
			if line.startswith("Sender: ") and started == True:
				count2 += 1
				ReadPosts[count].append("")
			ReadPosts[count][count2] = ReadPosts[count][count2] + line
			started = True
		count+=1

	count=0
	ADatesArray = []
	#Find Dates to be used as keys for All
	for stream in AllPosts:
		ADatesArray.append("")
		ADatesArray[count] = []
		for post in stream:
			tempDateFinder = post.splitlines()
			for line in tempDateFinder:
				if line.startswith("Date: "):
					ADatesArray[count].append(line)
					break
			count2+=1
		count+=1

	count=0
	#Find Dates to be used as keys for Unread
	for stream in UnreadPosts:
		UDatesArray.append("")
		UDatesArray[count] = []
		for post in stream:
			tempDateFinder = post.splitlines()
			for line in tempDateFinder:
				if line.startswith("Date: "):
					UDatesArray[count].append(line)
					break
			count2+=1
		count+=1

	count=0
	#Find Dates to be used as keys for Read
	for stream in ReadPosts:
		RDatesArray.append("")
		RDatesArray[count] = []
		for post in stream:
			tempDateFinder = post.splitlines()
			for line in tempDateFinder:
				if line.startswith("Date: "):
					RDatesArray[count].append(line)
					break
			count2+=1
		count+=1
	count=0

	AllAllPosts=[]
	#Add all unread posts to one array, then add all read posts to another array, then add all posts to an array
	for stream in UnreadPosts:
		for post in stream:
			AllUnreadPosts.append(post)
	for stream in ReadPosts:
		for post in stream:
			AllReadPosts.append(post)
	for stream in AllPosts:
		for post in stream:
			AllAllPosts.append(post)

	AllADates, SortedAll = [],[]
	#Do the same for the date strings
	for stream in UDatesArray:
		for date in stream:
			AllUDates.append(date)
	for stream in RDatesArray:
		for date in stream:
			AllRDates.append(date)
	for stream in ADatesArray:
		for date in stream:
			AllADates.append(date)

	#Removes any blank strings
	AllUnreadPosts = list(filter(None, AllUnreadPosts))
	AllReadPosts = list(filter(None, AllReadPosts))
	AllAllPosts = list(filter(None, AllAllPosts))
	AllUDates = list(filter(None, AllUDates))
	AllRDates = list(filter(None, AllRDates))
	AllADates = list(filter(None, AllADates))

	UnreadTuples = list(zip(AllUDates, AllUnreadPosts))
	ReadTuples = list(zip(AllRDates, AllReadPosts))
	AllTuples = list(zip(AllADates, AllAllPosts))

	#Sorts posts based on date
	UnreadTuples.sort()
	ReadTuples.sort()
	AllTuples.sort()

	#Splits the tuples into two lists of Read and Unread sorted posts
	SortedUnread = [x[1] for x in UnreadTuples]
	SortedRead = [x[1] for x in ReadTuples]
	SortedAll = [x[1] for x in AllTuples]

	postsRead=0
	for post in SortedRead:
		postsRead+=1

	numPosts=0
	for post in SortedAll:
		numPosts+=1

	#AllPostsSorted = SortedAll
	AllPostsSorted = SortedRead+SortedUnread

	#Split the posts into lines for printing
	for i in AllPostsSorted:
		TempAllPostLinesSorted.append(i.splitlines(True))

	for post in TempAllPostLinesSorted:
		for i in post:
			AllPostLinesSorted.append(i)

	maxlines = len(AllPostLinesSorted)
	argc = len(argv)
	currentPost = postsRead
	lineNum = 0
	count=0
	foundPost = False
	SenderCount = False

	#Find what line to start on
	for line in AllPostLinesSorted:
		if UnreadTuples:
			if line[ :-1] == UnreadTuples[0][0]:
				lineNum = count-1
				foundPost = True
				break
			count+=1
		else:
			break

	nametoggle = sys.argv[argc-1]

	if nametoggle == 'st@rt':
		# If the user clicked "Check for new posts, or if they are just starting"
		if decision == 'st@rt':
			if postsRead == numPosts:
				print ("\n----There are no more posts to show---- ")
				print(currentPost)
				exit()
			#byte = int(dataArray[postsRead])
			while (lineNum<maxlines):
				#Find if we are in a new post
				if AllPostLinesSorted[lineNum].startswith("Sender: "):
					if SenderCount == True:
						if postsRead < numPosts:
							postsRead += 1
						AddedDates = AddReadToCorrectFile(author, incStreamFiles, StreamsList, NumberRead, UnreadTuples, dateline, AddedDates)
						print(currentPost)
						exit()
					else:
						SenderCount = True
						dateline = AllPostLinesSorted[lineNum+1]
						aStreamName = FindStreamName(incStreamFiles, StreamsList, AllTuples, dateline)
						print ("Stream: " + aStreamName + "\n")
				print(AllPostLinesSorted[lineNum],end="")
				lineNum += 1
				if (lineNum == maxlines):
					postsRead = numPosts
					AddedDates = AddReadToCorrectFile(author, incStreamFiles, StreamsList, NumberRead, UnreadTuples, dateline, AddedDates)
					print(currentPost)
					exit()
			print("Program was unable to exit properly")
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
			lineNum=0
			count = 0
			SenderCount = False
			FirstFound = False
			#Set file to correct position
			while count < currentPost:
				if AllPostLinesSorted[lineNum].startswith("Sender: "):
					if FirstFound == True:
						count+=1
						if count == currentPost:
							break
					else:
						FirstFound = True
				lineNum+=1

			while (lineNum<maxlines):
				if AllPostLinesSorted[lineNum].startswith("Sender: "):
					if SenderCount == True:
						if postsRead < numPosts:
							postsRead += 1
						AddedDates = AddReadToCorrectFile(author, incStreamFiles, StreamsList, NumberRead, UnreadTuples, dateline, AddedDates)
						print(currentPost)
						exit()
					else:
						SenderCount = True
						dateline = AllPostLinesSorted[lineNum+1]
						aStreamName = FindStreamName(incStreamFiles, StreamsList, AllTuples, dateline)
						print ("Stream: " + aStreamName + "\n")

				print(AllPostLinesSorted[lineNum],end="")
				lineNum += 1
				if (lineNum == maxlines):
					postsRead = numPosts
					print(currentPost)
					AddedDates = AddReadToCorrectFile(author, incStreamFiles, StreamsList, NumberRead, UnreadTuples, dateline, AddedDates)
					exit()
		elif decision == '-m@ll':
			postsRead = numPosts
			SetAllFilesRead(incStreamFiles, author, StreamsDatasList)
			print ("\n----There are no more posts to show---- ")
			currentPost = postsRead
			print(currentPost)
			exit()

	elif nametoggle == 'nam3sort':
		while nametoggle != '-m@ll':
			currentPost = 0
			contentsLinebyLineSorted = []
			contentsLinebyLineSorted = SortByName("A Stream Name", AllPostLinesSorted, AllPostLinesSorted, AllAllPosts, True, incStreamFiles, StreamsList, AllTuples)
			maxlines = len(contentsLinebyLineSorted)
			lineNum = 0
			SenderCount = False
			# If the user clicked "Check for new posts, or if they are just starting"
			if decision == 'nam3sort':
				while (lineNum<maxlines):
					#Find if we are in a new post
					if contentsLinebyLineSorted[lineNum].startswith("Sender: "):
						if SenderCount == True:
							print(currentPost)
							exit()
						else:
							SenderCount = True
							dateline = AllPostLinesSorted[lineNum+1]
							aStreamName = FindStreamName(incStreamFiles, StreamsList, AllTuples, dateline)
							print ("Stream: " + aStreamName + "\n")
					print(contentsLinebyLineSorted[lineNum],end="")
					lineNum += 1
					if (lineNum == maxlines):
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
				lineNum=0
				count = 0
				SenderCount = FirstLine = False
				for line in contentsLinebyLineSorted:
					if count == currentPost:
						lineNum-=1
						break
					if line.startswith("Sender: "):
						if FirstLine == True:
							count += 1
						else:
							FirstLine = True
					lineNum+=1
				if lineNum < 0:
					lineNum = 0				

				while (lineNum<maxlines):
					if contentsLinebyLineSorted[lineNum].startswith("Sender: "):
						if SenderCount == True:
							print(currentPost)
							exit()
						else:
							SenderCount = True
							dateline = AllPostLinesSorted[lineNum+1]
							aStreamName = FindStreamName(incStreamFiles, StreamsList, AllTuples, dateline)
							print ("Stream: " + aStreamName + "\n")
					print(contentsLinebyLineSorted[lineNum],end="")
					lineNum += 1
					if (lineNum == maxlines):
						print(currentPost)
						exit()
			elif decision == '-m@ll':
				postsRead = numPosts
				SetAllFilesRead(incStreamFiles, author, StreamsDatasList)
				currentPost = 0
				#print(currentPost)
				decision = 'nam3sort'



if __name__ == "__main__":
	#Main Program
	userFiles, incStreamUFiles, incStreamFiles, dataArray, tempContents, contentsLinebyLine = [], [], [], [], [], []

	number = len(sys.argv)
	if len(sys.argv) < 3 | ((sys.argv[number-1] == 'un1qu3Ph@s3') & (len(sys.argv) < 3)):
		print ("You need to specify an author... Please Try Again")
		print('0')
		exit()

	if os.path.isdir('./messages/') == False:
		print ("There are currently no streams in the Database...Please return to the previous page")
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

	for file in os.listdir("./messages"):
		if file.endswith("Users"):
			userFiles.append(file)

	for i in userFiles:
		fileName = "./messages/" + i
		with open(fileName) as f:
			for line in f:
				if line.startswith(author):
					incStreamUFiles.append(i)
			f.close()

	#Checks if the user is a part of any streams
	if not incStreamUFiles:
		print ("The user is not included in any streams... Please Try Again")
		print('0')
		exit()

	for i in incStreamUFiles:
		incStreamFiles.append(i.replace('StreamUsers', ''))


	decision = ""
	start = False
	for i in sys.argv:
		if i == '-fl@g':
			break
		if start == True:
			decision = decision + i
		if i == 'un1qu3Ph@s3':
			start = True

	olddecision = decision
	argv = sys.argv
	if decision == 'all':
		#placeholder
		decision = ""
		start = False
		for i in sys.argv:
			if start == True:
				decision = i
				break
			if i == '-fl@g':
				start = True
		PrintAllStreams(incStreamFiles, incStreamUFiles, author, argv, decision)
		print('0')
		print("Shouldnt be here")
		exit()

	#Checks if the user made a valid choice of stream
	included = False
	for i in incStreamFiles:
		if decision == i:
			included = True

	if included != True:
		print ("Stream specified is not an option or does not exist... Please Try Again")
		print('0')
		exit()

	a = "0\n"
	numPosts = 0
	#Read in the Data File
	dataFile = "./messages/" + decision + "StreamData"
	with open(dataFile) as f:
		dataArray = f.readlines()
	f.close()

	origDataArray = dataArray

	for i in dataArray:
		numPosts += 1

	#Append 0 to beginning of data array representing 0 files read
	dataArray = [a] + dataArray

	contents = ""
	#Opens correct stream and saves to string "contents"
	streamPath = "./messages/" + decision + "Stream"
	with open(streamPath, "r") as f:
		contents = f.read()
	f.close()

	maxbyte = len(contents)
	with open(streamPath, "r") as f:
		contentsLinebyLine = f.readlines()
	f.close()

	maxlines = len(contentsLinebyLine)
	#Find where to start in the stream
	userFileName = streamPath + "Users"
	with open(userFileName, "r") as userFile:
		tempContents = userFile.readlines()
	userFile.close()

	postsRead = 0
	slicedLine = ""
	for line in tempContents:
		if line.startswith(author):
			slicedLine = line[66: ]
			postsRead = int(slicedLine)
			break
	dataArray.append("-400")

	#placeholder
	decision = ""
	start = False
	for i in sys.argv:
		if start == True:
			decision = i
			break
		if i == '-fl@g':
			start = True

	argc = len(sys.argv)
	currentPost = postsRead
	byte = 0
	lineNum = 0
	#Set file to correct position
	while byte < int(dataArray[postsRead]):
		byte += 1
		if contents[byte-1] == '\n':
			lineNum += 1
	nametoggle = sys.argv[argc-1]

	if nametoggle == 'st@rt':
		# If the user clicked "Check for new posts, or if they are just starting"
		if decision == 'st@rt':
			if postsRead == numPosts:
				print ("\n----There are no more posts to show---- ")
				print(currentPost)
				exit()
			#byte = int(dataArray[postsRead])
			while (lineNum<maxlines):
				#Find if we are in a new post
				for c in contentsLinebyLine[lineNum]:
					byte += 1
					if byte > int(dataArray[postsRead+1]) and int(dataArray[postsRead+1]) > 0:
						if postsRead < numPosts:
							postsRead += 1
						SaveReadPostsNum(author, olddecision, postsRead)
						print(currentPost)
						exit()
				print(contentsLinebyLine[lineNum],end="")
				lineNum += 1
				if (lineNum == maxlines):
					postsRead = numPosts
					print(currentPost)
					SaveReadPostsNum(author, olddecision, postsRead)
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
			byte=0
			lineNum=0
			SenderCount = False
			#Set file to correct position
			while byte < int(dataArray[currentPost]):
				byte += 1
				if contents[byte-1] == '\n':
					lineNum += 1

			while (lineNum<maxlines):
				#Find if we are in a new post
				for c in contentsLinebyLine[lineNum]:
					byte += 1
					if byte > int(dataArray[postsRead+1]) and int(dataArray[postsRead+1]) > 0:
						if postsRead < numPosts:
							postsRead += 1
						print(currentPost)
						SaveReadPostsNum(author, olddecision, postsRead)
						exit()
				if contentsLinebyLine[lineNum].startswith("Sender: "):
					if SenderCount == True:
						print(currentPost)
						exit()
					else:
						SenderCount = True
				print(contentsLinebyLine[lineNum],end="")
				lineNum += 1
				if (lineNum == maxlines):
					postsRead = numPosts
					print(currentPost)
					SaveReadPostsNum(author, olddecision, postsRead)
		elif decision == '-m@ll':
			postsRead = numPosts
			SaveReadPostsNum(author, olddecision, postsRead)
			print ("\n----There are no more posts to show---- ")
			currentPost = postsRead
			print(currentPost)
	elif nametoggle == 'nam3sort':
		while nametoggle != '-m@ll':
			currentPost = 0
			StreamsList = []
			AllTuples = []
			contentsLinebyLineSorted = []
			contentsLinebyLineSorted = SortByName(olddecision, contents, contentsLinebyLine, origDataArray, False, incStreamFiles, StreamsList, AllTuples)
			maxlines = len(contentsLinebyLineSorted)
			lineNum = 0
			SenderCount = False
			# If the user clicked "Check for new posts, or if they are just starting"
			if decision == 'nam3sort':
				while (lineNum<maxlines):
					#Find if we are in a new post
					if contentsLinebyLineSorted[lineNum].startswith("Sender: "):
						if SenderCount == True:
							print(currentPost)
							exit()
						else:
							SenderCount = True
					print(contentsLinebyLineSorted[lineNum],end="")
					lineNum += 1
					if (lineNum == maxlines):
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
				lineNum=0
				count = 0
				SenderCount = FirstLine = False
				for line in contentsLinebyLineSorted:
					if count == currentPost:
						lineNum-=1
						break
					if line.startswith("Sender: "):
						if FirstLine == True:
							count += 1
						else:
							FirstLine = True
					lineNum+=1	
				if lineNum < 0:
					lineNum = 0				

				while (lineNum<maxlines):
					if contentsLinebyLineSorted[lineNum].startswith("Sender: "):
						if SenderCount == True:
							print(currentPost)
							exit()
						else:
							SenderCount = True
					print(contentsLinebyLineSorted[lineNum],end="")
					lineNum += 1
					if (lineNum == maxlines):
						print(currentPost)
						exit()
			elif decision == '-m@ll':
				postsRead = numPosts
				SaveReadPostsNum(author, olddecision, postsRead)
				currentPost = 0
				#print(currentPost)
				decision = 'nam3sort'



			#for line in finalLines:
			#	print(line,end="")
			#print (currentPost)
