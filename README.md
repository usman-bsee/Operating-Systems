# Operating-Systems




# Qult-AI

# Airflow Setup Guide

## Docker Installation
We are installing the airflow with the help of dockers. So, to continue installation of airflow, we have to install and run dockers first. Steps to install dockers in ubuntu are as folow:

1. Download docker desktop from their official website.
2. Set up the repository
Update the `apt` package index and install packages to allow `apt` to use a repository over HTTPS:
```
$ sudo apt-get update
$ sudo apt-get install ca-certificates curl gnupg
```
3. Add Docker's official GPG key:
```
$ sudo install -m 0755 -d /etc/apt/keyrings
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
$ sudo chmod a+r /etc/apt/keyrings/docker.gpg
```
4. Use the following command to set up the repository:
```
$ echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```
5. Update the `apt` package index:
```
$ sudo apt-get update
```
6. Download the latest `deb` package
7. Install the package with apt as follows:
```
$ sudo apt-get install ./docker-desktop-<version>-<arch>.deb
```

To check if docker-desktop is successfully installed, check its version using the following command:
```
$ docker --version
```
### Launch Docker Desktop:
To start Docker Desktop for Linux, search Docker Desktop on the Applications menu and open it. Alternatively, open a terminal and run"
```
$ systemctl --user start docker-desktop
```

#### Once docker is enabled and in running state. Start installing airflow.


## Airflow Installation
1. Make new directory and change your directory to the new one.
```
$ mkdir airflow-docker
$ cd airflow-docker
```
2. Download `docker-compose.yaml` file using the following command:
```
$ curl -LfO 'http://apache-airflow-docs.s3-website.eu-central-1.amazonaws.com/docs/apache-airflow/latest/docker-compose.yaml'
```
3. Create new directories inside `airflow-docker` folder.
```
$ mkdir ./dags ./plugins ./logs
```
4. Create `.env` file
```
$ echo -e "AIRFLOW_UID=$(id -u)\nAIRFLOW_GID=0" > .env
```
5. Build the airflow services
```
$ docker-compose up airflow-init
```
6. Run the services specified in `.yaml` file.
```
$ docker-compose up
```

#### If you want to check which docker containers are up and running, open terminal and run this command.
```
$ docker ps
```

When you see the health of all the containers turn to `healthy`, open the browser and search `localhost:8080`. <br>
You'll see the login page of the airflow. The default username and password is `airflow`.


## Demo

Create two new files in the directory `dags` and save them as `choose_best_model.py` and `training_model.py`.
### Choose Best Model
Paste the following code into `choose_best_model.py`
```
def _choose_best_model(ti):
    accuracies = ti.xcom_pull(task_ids=[
        "training_model_A",
        "training_model_B",
        "training_model_C"
    ])
    best_accuracy = max(accuracies)
    if best_accuracy > 8:
        return "accurate"
    else:
        return "inaccurate"
```
### Training Model
Paste the following code into `training_model.py`
```
from random import randint

def _training_model():
    return randint(1, 10)
```
### Create a DAG
Create a new python file with the following code and save it as `dags/airflow_demo.py`:
```
from airflow import DAG
from airflow.operators.python import PythonOperator, BranchPythonOperator
from airflow.operators.bash import BashOperator
from random import randint
from datetime import datetime
from choose_best_model import _choose_best_model
from training_model import _training_model

with DAG("my_dag", start_date=datetime(2021, 1, 1), 
    schedule_interval="@daily", catchup=False) as dag:

    training_model_A = PythonOperator(
        task_id="training_model_A",
        python_callable=_training_model
    )

    training_model_B = PythonOperator(
        task_id="training_model_B",
        python_callable=_training_model
    )

    training_model_C = PythonOperator(
        task_id="training_model_C",
        python_callable=_training_model
    )
    
    choose_best_model = BranchPythonOperator(
        task_id="choose_best_model",
        python_callable=_choose_best_model
    )

    accurate = BashOperator(
        task_id="accurate",
        bash_command="echo 'accurate'"
    )

    inaccurate = BashOperator(
        task_id="inaccurate",
        bash_command="echo 'inaccurate'"
    )

    [training_model_A, training_model_B, training_model_C] >> choose_best_model >> [accurate, inaccurate]
```
#### After creating these three files, just reload the web page.
You'll see `my_dags` under the dags tab. Click on this dag, and then on the top left corner, you'll see the option to run the DAG. Click on it and you'll see the logs of the dag in the logs tab.
