# /********* index.py ***********/

import pandas as pd
import seaborn as sns

from sklearn.linear_model import LinearRegression
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import OneHotEncoder, LabelEncoder, StandardScaler

import matplotlib.pyplot as plt
import numpy as np

# print('version: {}'.format(pd.__version__))

#
input = input(
    "Enter the platform data to start (enter either `linux` or `macBook`): ")

# Load the Excel file
if (input == "macBook"):
    # pid,name,username,memory_percent,cpu_percent,nice,create_time,mem_rss,mem_vms,mem_pfaults,mem_pageins,num_ctx_switches_voluntary,num_ctx_switches_involuntary,utime,stime,cutime,cstime
    dataset = pd.read_csv('../data_collecting/darwin_processes.csv')
    X_drop_columns = ['pid', 'name', 'username', 'stime']
    y_column = 'stime'
    print("macBook")
elif (input == "linux"):
    # PID, NAME, ELAPSED_TIME, TOTAL_TIME, utime, stime, start_time, uptime
    dataset = pd.read_csv('../data_collecting/linux_log_file.csv')
    X_drop_columns = ['PID', 'NAME', 'stime']
    y_column = 'stime'
    print("linux")
else:
    print("Invalid input")

# print(dataset.columns)
# print(dataset.dtypes)

objTypeCols = dataset[[
    i for i in dataset.columns if dataset[i].dtype == 'object']]

# To find the correlation among the columns
corrprocessData = dataset.corr()
# print(corrprocessData)

# Dropping un-necessary information
#

# Label Encoding
# le = LabelEncoder() # LabelEncoder - Code categories into 0,1,2.....
# for i in objTypeCols.columns.values.tolist()[1:]:
#     dataset[i+"_coded"] = dataset[i].astype('category')
#     dataset[i+"_coded"] = le.fit_transform(dataset[i+"_coded"])

# One-Hot Encoding
# for i in objTypeCols.columns.values.tolist()[1:]:
#     dummyCols = pd.get_dummies(dataset[i], prefix=i) # Convert 1/0 based on presence; C no. of columns
#     dataset = dataset.join(dummyCols)
# ohe = OneHotEncoder()

# Assigning columns variables to X and y
X = dataset.drop(X_drop_columns, axis=1)
# utime: CPU time spent in user code
# stime: CPU time spent in kernel code
y = dataset[y_column]

# Model building
# test_size: 25% of the data will go to the test set, whereas the remaining 75% to the training set
# random_state: using this parameter makes sure that anyone who re-runs your code will get the exact same outputs.
# Popular integer random seeds are 0 and 42.
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.25, random_state=42)
# print(X_train)
# print(X_test)
# print(y_train)
# print(y_test)

######################################### Linear Regression ###############################################
lr = LinearRegression()

lr.fit(X_train, y_train)

y_lr_train_pred = lr.predict(X_train)
y_lr_test_pred = lr.predict(X_test)

# Model performance
# variables contain the performance metrics MSE and R2 for models build using linear regression on the training set
# MSE: the squared distance between actual and predicted values. Squared to avoid the cancellation of negative terms.
lr_train_mse = mean_squared_error(y_train, y_lr_train_pred)
lr_train_r2 = r2_score(y_train, y_lr_train_pred)

lr_test_mse = mean_squared_error(y_test, y_lr_test_pred)
lr_test_r2 = r2_score(y_test, y_lr_test_pred)

lr_results = pd.DataFrame(['Linear regression', lr_train_mse,
                          lr_train_r2, lr_test_mse, lr_test_r2]).transpose()
lr_results.columns = ['Method', 'Training MSE',
                      'Training R2', 'Test MSE', 'Test R2']
print(lr_results)
print("Test R2 / Accuracy:", lr_test_r2*100, '%')
print("Test RMSE: ", np.sqrt((lr_test_mse)))

######################################### Random Forest ###############################################
# build a random forest with 1000 decision trees
rr = RandomForestRegressor(n_estimators=1000, random_state=42)

rr.fit(X_train, y_train)

y_rr_train_pred = rr.predict(X_train)
y_rr_test_pred = rr.predict(X_test)

# Model performance
# variables contain the performance metrics MSE and R2 for models build using random forest on the training set
rr_train_mse = mean_squared_error(y_train, y_rr_train_pred)
rr_train_r2 = r2_score(y_train, y_rr_train_pred)

rr_test_mse = mean_squared_error(y_test, y_rr_test_pred)
rr_test_r2 = r2_score(y_test, y_rr_test_pred)

rr_results = pd.DataFrame(['Random forest', rr_train_mse,
                          rr_train_r2, rr_test_mse, rr_test_r2]).transpose()
rr_results.columns = ['Method', 'Training MSE',
                      'Training R2', 'Test MSE', 'Test R2']
print(rr_results)
print("Test R2 / Accuracy:", rr_test_r2*100, '%')
print("Test RMSE: ", np.sqrt((rr_test_mse)))

#
# print(y_train)
# print(y_lr_train_pred)
# print(y_rr_train_pred)

# Data visualization of prediction results for lr
plt.figure(figsize=(5, 5))
plt.scatter(x=y_train, y=y_lr_train_pred, c="#7CAE00", alpha=0.3)
# A trend line to the plot
z = np.polyfit(y_train, y_lr_train_pred, 1)
# print(z)
p = np.poly1d(z)
plt.plot(y_train, p(y_train), "#F8766D")
plt.title('Using linear regression', fontsize=15)
plt.ylabel('Predicted')
plt.xlabel('Experimental')
plt.show()

# Data visualization of prediction results for rr
plt.figure(figsize=(5, 5))
plt.scatter(x=y_train, y=y_rr_train_pred, c="#7CAE00", alpha=0.3)
# A trend line to the plot
z = np.polyfit(y_train, y_rr_train_pred, 1)
# print(z)
p = np.poly1d(z)
plt.plot(y_train, p(y_train), "#F8766D")
plt.title('Using random forest regressor', fontsize=15)
plt.ylabel('Predicted')
plt.xlabel('Experimental')
plt.show()
