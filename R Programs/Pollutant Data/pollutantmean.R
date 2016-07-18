###############################################################
# Takes in the directory as a string and the id of the current
# file needing to be accessed, then returns that filename
#
# Parameters:
# directory:	A string containing the subdirectory of the 
#		file to be opened
# id:		A numeric representing which monitoring station
#		file to open
###############################################################

getFileName <- function(directory, id) {
    fileList <- list.files(directory, full.names = TRUE)
    filename <- fileList[id]
}

###############################################################
# Takes the pollutant parameter and finds the mean of all
# values for that pollutant in every monitoring station provided
# by id, then returns that mean as a numeric vector
#
# Parameters:
# directory:	A string containing the subdirectory of the 
#		file to be opened
# pollutant:	A string that is either "nitrate" or "sulfate"
# id:		A numeric representing which monitoring station
#		file to open 
###############################################################

pollutantmean <- function(directory, pollutant, id = 1:332) {
    finalVector <- numeric(0)
    for(i in 1:length(id)) {
        filename <- getFileName(directory, id[i])
        currentData <- read.csv(filename)
        tempVector <- currentData[[pollutant]]
        tempVector <- tempVector[!is.na(tempVector)]
        finalVector <- c(finalVector, tempVector)
    }
    mean(finalVector)
}