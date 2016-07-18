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
# Takes in the directory as a string and evaluates the
# correlation between nitrate and sulfate in every monitoring
# station with complete cases above the threshold parameter,
# then returns a numeric vector of all qualifying correlations
#
# Parameters:
# directory:	A string containing the subdirectory of the 
#		file to be opened
# threshold:	The number of complete cases necessary between
#		nitrate and sulfate in a single monitoring 
#		station for that station to be evaluated
###############################################################

corr <- function(directory, threshold = 0) {
    corrVector <- numeric(0)
    
    for(i in 1:332) {
        filename <- getFileName(directory, i)
        currentData <- read.csv(filename)
        if(sum(complete.cases(currentData$nitrate, currentData$sulfate)) > threshold) {
            corrVector <- c(corrVector, 
		cor(currentData$nitrate, currentData$sulfate, use = "complete.obs"))
        }
    }
    corrVector
}