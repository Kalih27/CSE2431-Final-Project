""" Import library """
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from datetime import datetime

from sklearn.preprocessing import OneHotEncoder, LabelEncoder, StandardScaler


def data_preprocessing(platform, df):
    """ Get obj cols and obj-col df """
    obj_type_cols = [
        i for i in df.columns if df[i].dtype == 'object']
    if (platform == "macBook"):
        obj_type_cols.append("pid")
    elif (platform == "linux"):
        obj_type_cols.append("PID")
    df_obj_type_cols = df[obj_type_cols]

    """ Show correlation plot """
    corr = df.corr()
    # plt.figure(figsize=(12, 4))
    # sns.heatmap(corr, cmap='Wistia', annot=True)
    # plt.title('Correlation in the dataset')
    # plt.show()

    # 1. Label Encoding
    le = LabelEncoder()  # Code categories into 0,1,2.....
    updated_df = df
    for col in df_obj_type_cols:
        updated_df[col] = le.fit_transform(updated_df[col])
    # print(updated_df.head())

    # 2. One-Hot Encoding (Not best work in this case)
    # ohe = OneHotEncoder()
    # for col in df_obj_type_cols:
    #     # Reshape the 1-D array to 2-D as fit_transform expects 2-D and finally fit the object
    #     ohe_data = ohe.fit_transform(
    #         updated_df[col].values.reshape(-1, 1)).toarray()
    #     # Add this back into the original dataframe
    #     ohe_df = pd.DataFrame(
    #         ohe_data, columns=[col+"_"+str(int(i)) for i in range(ohe_data.shape[1])])
    #     updated_df = pd.concat([updated_df, ohe_df], axis=1)
    # # Drop the obj columns
    # updated_df = updated_df.drop(df_obj_type_cols, axis=1)

    # 3. Dummy variable trap (Not best work in this case)
    # updated_df = pd.get_dummies(data=updated_df, prefix='OHE', prefix_sep='_',
    #                            columns=obj_type_cols,
    #                            drop_first=True,
    #                            dtype='int8')

    """ Drop un-necessary information such as all 0 in the same col """
    for col in updated_df.columns:
        if (updated_df[col] == 0).all(axis=0):
            updated_df = updated_df.drop(col, axis=1)

    """ Check missing value """
    # plt.figure(figsize=(12, 4))
    # sns.heatmap(updated_df.isnull(), cbar=False, cmap='viridis', yticklabels=False)
    # plt.title('Missing value in the dataset')
    # plt.show()

    # print(df)
    # print(updated_df)

    return updated_df
