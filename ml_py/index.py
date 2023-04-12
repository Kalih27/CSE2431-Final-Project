# /********* index.py ***********/

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from math import sqrt

from sklearn.linear_model import LinearRegression
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error


# Load the Excel file
dataset = pd.read_csv('../sudo_py_mac/mac_processes.csv')

x = dataset.iloc[:, 5:-2].values
y = dataset.iloc[:, -2].values
# print(processData.columns)
# print(processData.dtypes)
# print(x)
# print(y)
objTypeCols = dataset[[
    i for i in dataset.columns if dataset[i].dtype == 'object']]

#
corrprocessData = dataset.corr()
# print(corrprocessData)

# Dropping un-necessary information

# Model building
x_train, x_test, y_train, y_test = train_test_split(
    x, y, test_size=0.25, random_state=0)
######################################### Linear Regression ###############################################
lr = LinearRegression()

lr.fit(x_train, y_train)

y_pred = lr.predict(x_test)
r2_score = lr.score(x_test, y_test)
print("Accuracy1:", r2_score*100, '%')
print("RMSE1: ", sqrt(mean_squared_error(y_test, y_pred)))

# predicting value
new_prediction = lr.predict(np.array([[1100, 768, 2000, 0, 1, 1]]))
print("Prediction performance:", float(new_prediction))
new_prediction = lr.predict(np.array([[143, 512, 5000, 0, 7, 32]]))
print("Prediction performance:", float(new_prediction))
new_prediction = lr.predict(np.array([[64, 5240, 20970, 30, 12, 24]]))
print("Prediction performance:", float(new_prediction))
new_prediction = lr.predict(np.array([[700, 256, 2000, 0, 1, 1]]))
print("Prediction performance:", float(new_prediction))

######################################### Random Forest ###############################################
Rr = RandomForestRegressor(n_estimators=50, max_features=None, random_state=0)

Rr.fit(x_train, y_train)

y_pred = Rr.predict(x_test)
r2_score = Rr.score(x_test, y_test)
print("Accuracy1:", r2_score*100, '%')
print("RMSE1: ", sqrt(mean_squared_error(y_test, y_pred)))

# predicting value
new_prediction = Rr.predict(np.array([[1100, 768, 2000, 0, 1, 1]]))
print("Prediction performance:", float(new_prediction))
new_prediction = Rr.predict(np.array([[143, 512, 5000, 0, 7, 32]]))
print("Prediction performance:", float(new_prediction))
new_prediction = Rr.predict((np.array([[64, 5240, 20970, 30, 12, 24]])))
print("Prediction performance:", float(new_prediction))
new_prediction = Rr.predict((np.array([[700, 256, 2000, 0, 1, 1]])))
print("Prediction performance:", float(new_prediction))
