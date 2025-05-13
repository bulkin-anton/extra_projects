#import all necessary functions and methods
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
from sklearn.datasets import fetch_covtype

#import dataset from the library
database = fetch_covtype()

#prepare dataset - split for test and train parts
database_train, database_test, result_train, result_test = train_test_split(database.data, database.target, train_size = 0.7,  test_size=0.3)

from sklearn.linear_model import SGDClassifier

# list of all loss functions used in task
loss_functions = ['hinge',
         'log_loss',
         'modified_huber',
         'squared_hinge',
         'perceptron',
         'squared_error',
         'huber',
         'epsilon_insensitive',
         'squared_epsilon_insensitive'
]

#take function with number 0, for example
lfunction = loss_functions[0]

#traininng of the model
sgd = SGDClassifier(loss = lfunction)
sgd.fit(database_train, result_train)

#predict for test part of dataset
result_pred = sgd.predict(database_test)

#calculate the value of the quality metric
accuracy = accuracy_score(result_test, result_pred)

#output of a result with the value of quality metric
print(lfunction)
print("Accuracy: ", accuracy)
print(classification_report(result_test, result_pred, target_names=['1', '2', '3', '4', '5', '6', '7']))


from sklearn.model_selection import GridSearchCV
from sklearn.tree import DecisionTreeClassifier

# list of all scoring functions used in task
scoring_functions = ['jaccard_weighted',
                     'f1_weighted',
                     'top_k_accuracy',
                     'precision_weighted',
                     'recall_weighted',
                     'roc_auc_ovr_weighted',
                     'roc_auc_ovr']

#take function with number 0, for example
sfunction = scoring_functions[0]

#list of parametrs used in GrindSearchCV to find best ones
parameters = {'criterion': ('gini', 'entropy', 'log_loss'),  'min_samples_split':[num for num in range(2, 31)]}

#traininng of the model
knc = DecisionTreeClassifier()
gscv = GridSearchCV(knc, parameters, scoring = sfunction)
gscv.fit(database_train, result_train)

#predict for test part of dataset
y_pred = gscv.predict(database_test)

#calculate the value of the quality metric
accuracy = accuracy_score(result_test, result_pred)

#output of a result with the value of quality metric and
# best parametrs for selected scoring functions
print(sfunction)
print("Accuracy: ", accuracy)
print('BEST PARAMS: ', gscv.best_params_)
print(classification_report(result_test, result_pred, target_names=['1', '2', '3', '4', '5', '6', '7']))