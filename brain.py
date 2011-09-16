import requests
import string
import re
import urllib2
import sys

SN = "SEND_NOTHING"

def remove_html_tags(data):
    p = re.compile(r'<.*?>')
    return p.sub('', data)
    
def remove_bracket_tags(data):
    p = re.compile(r'\[[^]]+]')
    return p.sub('', data)
    
def brain(raw_query = "c plus plus"):
	query = urllib2.quote(raw_query)
	r = requests.get('http://en.wikipedia.org/wiki/Special:Export?search='+query+'&go=Go')
	html_content = r.content
	first_paragraph = ""
	for i in range(html_content.index("<p>"),html_content.index("</p>")):
		first_paragraph += html_content[i]
	return remove_bracket_tags(remove_html_tags(first_paragraph))
	
def stripFromEnd(victim,x):
	toReturn = ""
	for i in range(0,len(victim)-x):
		toReturn += victim[i]
	return toReturn
  
recieved = ""
query = ""
failedSearch = "For search options, see Help:Searching."
ambiguity = "may refer to:"

for i in range(1,len(sys.argv)):
	recieved += str(sys.argv[i]+" ")

def handleInput(String,NumberOfWords):
	params = String.split();
	q = ""
	for i in range(NumberOfWords,len(params)):
		q += str(params[i]+" ")
	query = q
	return q
	
def handleBrain(Number):
	information = brain(handleInput(recieved,Number))
	testFS = stripFromEnd(information,1)
	if ( testFS == failedSearch ):
		print "Sorry, I couldn't find anything about that."
	elif (information.find(ambiguity) != -1):
		print "Please be a little more specific."
	else:
		print "'"+information+"'"

if (recieved.find("Navn, what is a") != -1):
	handleBrain(4)
elif (recieved.find("Navn, what is") != -1):
	handleBrain(3)
elif(recieved.find("Navn, what are the") != -1):
	handleBrain(4)		
elif (recieved.find("Navn, what are") != -1):
	handleBrain(3)
elif (recieved.find("Navn, can you tell me about the") != -1):
	handleBrain(7)
elif (recieved.find("Navn, can you tell me about a") != -1):
	handleBrain(7)
elif (recieved.find("Navn, can you tell me about") != -1):
	handleBrain(6)

else:
	print SN
	
