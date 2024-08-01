# Реализирайте Наивен Бейсов Класификатор, класифициращ индивидите като демократи или републиканци, използвайки 16-те 
# атрибута и двата класа от следните данни: http://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records

# За тестване на алгоритъма приложете 10-fold крос-валидация (разделете данните по случаен начин на 10 множества и 
# направете 10 обучения и тествания на модела като всеки път използвате едно от множествата за тестване, а останалите 9 
# за обучение).

# Изведете метриката точност (Accuracy) за всяко от 10-те обучения, както и тяхното средно аритметично (за обобщена
# оценка на класификатора).

# * Данните може да имат нужда от предварителна обработка.
# ** Справете се с проблемите с нулевите вероятности като използвате "Заглаждане на Лаплас" и логаритъм.

setwd("C:\\Users\\Fujitsu\\Documents\\Data\\Laptop\\FMI_ComputerScience\\2.Artificial Intelligence\\Practice\\Programs")

install.packages("e1071")
library(e1071)
library(mlbench)
data(HouseVotes84, package = "mlbench")

nbc_classifier <- function() {
  df <- HouseVotes84[(1:430), ] # [(431:435),] will be added later
  new_df <- df[sample(nrow(df)), ]

  accuracy <- 0

  for (x in 0:9) {
    training <- new_df[-((43 * x + 1):(43 * x + 43)), ]

    if (x != 9) {
      training <- rbind(training, HouseVotes84[(431:435), ])
    }

    testing <- new_df[((43 * x + 1):(43 * x + 43)), ]

    if (x == 9) {
      testing <- rbind(testing, HouseVotes84[(431:435), ])
    }

    # naiveBayes -> na.action -> by default does not count the values
    classifier <- naiveBayes(Class ~ ., data = training, laplace = 1,
                  log = TRUE, na.action = na.omit)
    predictions <- predict(classifier, newdata = testing)

    m <- mean(predictions == testing$Class)
    print(m)

    accuracy <- accuracy + m
  }

  print("Arithmetic mean")
  return(accuracy / 10)
}

nbc_classifier()

# mytable = table(testing$Class, predictions)
# myproptable = prop.table(mytable)
# success = myproptable[1,1] + myproptable[2,2]

# arithmetic mean -> some results: 0.9012685 | 0.9033827 | 0.9057082 | 0.9078224 | 0.9104651

# laplace    -> fix problem with zero probabilities
# log = TRUE -> probabilities p are given as log(p).
# na.action  -> na.omit returns the object with incomplete cases removed