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
# Takes in the directory as a string and the id of the current
# monitoring stations to be accessed, returns a data frame with
# the first column (id) as an id for each monitoring station, 
# and the second column (nobs) showing the number of complete
# cases between nitrate and sulfate
#
# Parameters:
# directory:	A string containing the subdirectory of the 
#		file to be opened
# id:		A numeric representing which monitoring station
#		files to open
###############################################################

complete <- function(directory, id = 1:332) {
    nobsVector <- numeric(0)
    
    for(i in 1:length(id)) {
        filename <- getFileName(directory, id[i])
        currentData <- read.csv(filename)
        currentNobs <- sum(complete.cases(currentData$nitrate, currentData$sulfate))
        nobsVector <- c(nobsVector, currentNobs)
    }
    data.frame(id = id, nobs = nobsVector)
}