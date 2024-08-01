# Използвайте алгоритъма kMeans за да се определят автоматично групи от сходни елементи (клъстери) за наборите от данни 
# "normal.txt" и "unbalance.txt".
# Вход: име на файл и брой клъстери.
# Изход: Картинка, която показва различните клъстери в различни цветове.
# (Всички примери от приложените бази са описани чрез два атрибута – x и у, които са разположението на точката в ЕП)

# * В решението приложете Random Restart с оценка колко са добри постигнатите клъстери.
# Като оценка можете да ползвате "вътрешно клъстерно разстояние", "междуклъстърно разстояние" и комбинация от двете.
# Опитайте се да сравните резултатите (можете да сравните и с оценка различна от предложените).
# ** Като допълнение можете да приложите kMeans++ и да сравните резултатите.
# *** Като допълнение можете да приложите Soft kMeans и да сравните резултатите.

setwd("C:\\Users\\Fujitsu\\Documents\\Data\\Laptop\\FMI_ComputerScience\\2.Artificial Intelligence\\Practice\\Programs")

install.packages("factoextra")
library(factoextra)

f <- function(filename, separate, k, ns) {
  df <- read.csv2(filename, header = FALSE, sep = separate, dec = ".")

  # nstart -> random starts
  km_result <- kmeans(df, centers = k, nstart = ns)

  #km_result$withinss # Vector of within-cluster sum of squares, one component per cluster
  #km_result$tot.withinss #Total within-cluster sum of squares, i.e. sum(withinss)
  #km_result$betweenss #The between-cluster sum of squares
  #km_result$centers #A matrix of cluster centers
  #print(km_result$size)

  fviz_cluster(km_result, data = df)
}

f("kmeans-normal.txt", "\t", 4, 10)
f("kmeans-unbalance.txt", " ", 8, 10)

unbalance_reduced <- function(k, ns) {
  df_unbalance = read.csv2("kmeans-unbalance.txt", header = FALSE, sep = " ", dec = ".")
  # clusters -> (first, second, third, other 5) -> (1:2000, 2001:4000, 4001:6000, 6001:6500)
  # first, second, third -> 2000 points each; other 5 -> 100 points each
  df_reduced <- rbind(df_unbalance[sample(1:2000, 100, replace = FALSE), ],
                     df_unbalance[sample(2001:4000, 100, replace = FALSE), ],
                     df_unbalance[sample(4001:6000, 100, replace = FALSE), ],
                     df_unbalance[(6001:6500), ])

  km_result <- kmeans(df_reduced, centers = k, nstart = ns)

  fviz_cluster(km_result, data = df_reduced)
}

unbalance_reduced(8, 10)

df_normal = read.csv2("kmeans-normal.txt", header = FALSE, sep = "\t", dec = ".")
df_unbalance = read.csv2("kmeans-unbalance.txt", header = FALSE, sep = " ", dec = ".")

# elbow_method
fviz_nbclust(df_normal, kmeans, method = "wss")
# fviz_nbclust(df_unbalance, kmeans, method = "wss") # k=8
fviz_nbclust(df_reduced, kmeans, method = "wss") # k=8
