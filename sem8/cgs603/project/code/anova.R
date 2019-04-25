require(dplyr)


 #*******************FULL*********************
df <- read.csv("data/collated.csv")

df <- df[which(df$type == "fu"), ]

df2 <- df[ which(df$video == "ba"), ]
df <- df[which(df$video != "ba"), ]

df$incorrect <- (df$response != "pa") & (df$response != "ba")
df2$bias <- (df2$response != "pa") & (df2$response != "ba")

df <- df %>% group_by(subject_id, gender) %>% summarise(incorrect = mean(incorrect))
df2 <- df2 %>% group_by(subject_id, gender) %>% summarise(bias = mean(bias))

df <- inner_join(df, df2, on=c('subject_id', 'gender'))

df$incorrect <- df$incorrect - df$bias
df <- replace(df, df < 0, 0)

fit <- aov(incorrect ~ gender, data =df)
print("BREIF")
print(summary(fit))
print(model.tables(fit,"means"),digits=3)       #report the means and the number of subjects/cell


# *******************BRIEF*****************************

df <- read.csv("data/collated.csv")

df <- df[which(df$type != "fu"), ]

df2 <- df[ which(df$video == "ba"), ]
df <- df[which(df$video != "ba"), ]

df$incorrect <- (df$response != "pa") & (df$response != "ba")
df2$bias <- (df2$response != "pa") & (df2$response != "ba")

df <- df %>% group_by(subject_id, gender) %>% summarise(incorrect = mean(incorrect))
df2 <- df2 %>% group_by(subject_id, gender) %>% summarise(bias = mean(bias))

df <- inner_join(df, df2, on=c('subject_id', 'gender'))

df$incorrect <- df$incorrect - df$bias
df <- replace(df, df < 0, 0)

fit <- aov(incorrect ~ gender, data =df)
print("BREIF")
print(summary(fit))
print(model.tables(fit,"means"),digits=3)       #report the means and the number of subjects/cell
