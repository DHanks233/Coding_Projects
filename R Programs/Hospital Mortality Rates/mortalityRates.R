# These functions use two .csv's provided by the US government to rank 
# hospitals in each state, or in all states, by their mortality rate in one
# of three categories: heart attack, heart failure or pneumonia.


##########################################################################
# Takes in a string representing the state and another string representing
# the outcome of care/cause of death, and returns the name of the hospital
# in that state with the lowest  mortality rate by that cause.
#
# Parameters:
# state:	A string abbreviation of the state ("TX" or "WA" or "CA")
# outcome:	The mortality rate to use (heart attack, heart failure or
#		pneumonia).
#
##########################################################################

best <- function(state, outcome) {
	causeOfDeath <- c("heart attack" = 11, "heart failure" = 17, 
		"pneumonia" = 23)
	outcome <- tolower(outcome)
	causeValidation(outcome, names(causeOfDeath))
	state <- toupper(state)
	data <- read.csv("outcome-of-care-measures.csv", na.strings = 
		"Not Available", stringsAsFactors = FALSE)
	ranks <- data.frame("name" = as.character(data[, 2]), "state" = 
		data[, 7], "deaths" = data[, causeOfDeath[outcome]])
	stateValidation(state, unique(ranks$state))
	ranks <- ranks[complete.cases(ranks), ]
	ranks <- ranks[ranks[, 2] == state, ]
	ranks <- ranks[order(ranks[, 2], ranks[, 3], ranks[, 1]),]
	as.character(head(ranks, 1)[, 1])
}

##########################################################################
# Takes in a string representing the state and another string representing
# the outcome of care/cause of death, and returns the name of the hospital
# in that state with the mortality rate that has the rank provided by num
#
# Parameters:
# state:	A string abbreviation of the state ("TX" or "WA" or "CA")
# outcome:	The mortality rate to use (heart attack, heart failure or
#		pneumonia).
# num:		The rank of the hospital to return. "best" = 1, or "worst"
#		are acceptable input
#
##########################################################################

rankhospital <- function(state, outcome, num = "best") {
	causeOfDeath <- c("heart attack" = 11, "heart failure" = 17, 
		"pneumonia" = 23)
	outcome <- tolower(outcome)
	causeValidation(outcome, names(causeOfDeath))
	state <- toupper(state)
	if(!is.numeric(num)) {
		num <- tolower(num)
	}
	data <- read.csv("outcome-of-care-measures.csv", na.strings = 
		"Not Available", stringsAsFactors = FALSE)
	ranks <- data.frame("name" = as.character(data[, 2]), "state" = 
		data[, 7], "deaths" = data[, causeOfDeath[outcome]])
	stateValidation(state, unique(ranks$state))
	ranks <- ranks[complete.cases(ranks), ]
	ranks <- ranks[ranks[, 2] == state, ]
	ranks <- ranks[order(ranks[,2], ranks[,3], ranks[,1]),]
	if(num == "best") {
		name <- as.character(head(ranks, 1)[, 1])
	}
	else if(num == "worst") {
		name <- as.character(tail(ranks, 1)[, 1])
	}
	else {
		name <- as.character(ranks[num, 1])
	}
	name
}

##########################################################################
# Takes in a string representing the outcome of care/cause of death, and 
# returns a data frame with the first column as each state, and the second
# column as the hospital in that state with the mortality rank provided by
# the num parameter
#
# Parameters:
# outcome:	The mortality rate to use (heart attack, heart failure or
#		pneumonia).
# num:		The rank of the hospital to return. "best" = 1, or "worst"
#		are acceptable input
#
##########################################################################

rankall <- function(outcome, num = "best") {
	causeOfDeath <- c("heart attack" = 11, "heart failure" = 17, 
		"pneumonia" = 23)
	outcome <- tolower(outcome)
	causeValidation(outcome, names(causeOfDeath))
	if(!is.numeric(num)) {
		num <- tolower(num)
	}
	data <- read.csv("outcome-of-care-measures.csv", na.strings = 
		"Not Available", stringsAsFactors = FALSE)
	ranks <- data.frame("name" = as.character(data[, 2]), "state" = 
		data[, 7], "deaths" = data[, causeOfDeath[outcome]])
	ranks <- ranks[complete.cases(ranks), ]
	ranks <- ranks[order(ranks[,2], ranks[,3], ranks[,1]),]
	allranks <- data.frame("hospital" = character(50), "state" = 
		character(50), stringsAsFactors = FALSE)
	states <- unique(ranks$state)
	states <- sort(states)

	for(i in 1:length(states)) {
		currentState <- ranks[ranks[, 2] == states[i], ]
		currentState <- currentState[order(currentState[,3], 
			currentState[,1]), ]
		allranks[i,2] <- as.character(states[i])
		if(num == "best") {
			allranks[i,1] <- as.character(currentState[1,1])
		}
		else if(num == "worst") {
			allranks[i,1] <- as.character(tail(currentState, 1)[, 1])
		}
		else {
			allranks[i,1] <- as.character(currentState[num, 1])
		}
	}
	allranks
}

##########################################################################
# An input validation function that calls a stop() if the input provided
# by user in the currentState parameter is not contained in a list of all
# legitimate state abbreviations (allStates parameter)
#
# Parameters:
# currentState:	A string abbreviation of the state ("TX" or "WA" or "CA")
# allStates:	A list of all possible state abbreviations that are correct
#
##########################################################################

stateValidation <- function(currentState, allStates) {
	if(!any(currentState == allStates)) {
		stop("Incorrect state input.")
	}
}

##########################################################################
# An input validation function that calls a stop() if the input provided
# by user in the currentCause parameter is not contained in a list of all
# legitimate causes (causesOfDeath parameter)
#
# Parameters:
# currentCause:	A string representing user input cause of death
# causesOfDeath:A list of all possible correct causes of death
#
##########################################################################

causeValidation <- function(currentCause, causesOfDeath) {
	if(!any(currentCause == causesOfDeath)) {
		stop("Incorrect cause of death input.")
	}
}