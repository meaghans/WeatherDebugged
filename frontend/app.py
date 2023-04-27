from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

@app.route('/')
def index():
    return render_template("climateSearch.html", methods = ['GET', 'POST'])

@app.route('/getInformation', methods = ['POST'])
def getInformation():
    data = request.form
    arguments = ["./program.exe"] + cmd_line_arguments(data).split(" ")
    print(arguments)
    result = subprocess.run(arguments, capture_output=True, text=True)
    return render_template("climateResult.html", data = result.stdout)

def cmd_line_arguments(data):
    arg = ""
    data_keys = ["sortMethod", "tempMin", "tempMax", "rainfallMin", "rainfallMax", "humidityMin", "humidityMax", "tempTrend", "climateType"]
    for key in data_keys:
        arg += data[key] + " "
    arg = arg[:-1]
    return arg

if __name__=='__main__':
    app.run(debug=True)