#library(readr)
library(reshape2)
library(e1071)

# Reading data frames
train <- read.csv("../train.csv")
test <- read.csv("../test.csv")

# Test: Tomo los campos relevantes
test <- subset(test, select = c("Id", "PdDistrict"))

# NaiveBayes classifier
classifier <- naiveBayes(train$Category, train$PdDistrict)
# submission <- table(predict(classifier, test), test$PdDistrict)

crimes <- as.data.frame.matrix(classifier$tables$x)

# Agrego campo by merge
crimes$PdDistrict <- rownames(crimes)
submission <- merge(test, crimes, by = "PdDistrict")
submission$PdDistrict <- NULL

# Remove variables to create the submission file
rm(train)
rm(test)
rm(classifier)
rm(crimes)

# Write CSV
write.table(submission, file = "submission.csv",row.names=FALSE, na="",col.names=TRUE, sep=",")

#fileOut <- gzfile("submit.csv.gz", "w")
#writeChar(write.csv(submission, ","), fileOut, eos=NULL)
#close(fileOut)
