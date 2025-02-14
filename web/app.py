from flask import Flask, request, render_template
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app)

sensor_data = []
errors = []

@app.route('/update', methods=['POST'])
def update():
    distance1 = request.form.get('distance1')
    ledStatus1 = request.form.get('ledStatus1')
    distance2 = request.form.get('distance2')
    ledStatus2 = request.form.get('ledStatus2')
    distance3 = request.form.get('distance3')
    ledStatus3 = request.form.get('ledStatus3')
    freeSpaces = request.form.get('freeSpaces')
    if distance1 and ledStatus1 and distance2 and ledStatus2 and distance3 and ledStatus3 and freeSpaces:
        data = {
            'distance1': distance1,
            'ledStatus1': ledStatus1,
            'distance2': distance2,
            'ledStatus2': ledStatus2,
            'distance3': distance3,
            'ledStatus3': ledStatus3,
            'freeSpaces': freeSpaces
        }
        sensor_data.append(data)
        print(f"Received data: {data}")
        socketio.emit('update_data', data)
        print(f"Emitted update_data event with data: {data}")
    return "Received", 200

@app.route('/error', methods=['POST'])
def error():
    error_msg = request.form.get('error')
    if error_msg:
        errors.append(error_msg)
    return "Error Received", 200

@app.route('/')
def index():
    latest_data = sensor_data[-1] if sensor_data else None
    return render_template('index.html', latest_data=latest_data, errors=errors)

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)
