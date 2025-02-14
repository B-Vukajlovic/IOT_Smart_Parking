document.addEventListener('DOMContentLoaded', function () {
    console.log('Document is ready');

    var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
    console.log('Attempting to connect to WebSocket');

    socket.on('connect', function() {
        console.log('WebSocket connected');
    });

    socket.on('update_data', function(latestData) {
        console.log('Received update_data event:', latestData);

        const parkingRows = document.querySelectorAll('.parking-row');

        const freeSpaces = parseInt(latestData.freeSpaces, 10);

        console.log('Free spaces:', freeSpaces);

        const freeElement = document.getElementById('free');

        if (freeElement && freeElement.querySelector('p')) {
            freeElement.querySelector('p').textContent = freeSpaces;
        } else {
            console.error('Free element not found or does not contain a paragraph');
        }

        parkingRows.forEach((row, index) => {
            let ledStatus = latestData[`ledStatus${index + 1}`];
            let existingImg = row.querySelector('img.car');

            if (ledStatus === 'OFF' && !existingImg) {
                const img = document.createElement('img');
                img.src = '/static/car.png';
                img.alt = 'Sliding Car Image';
                img.width = 240;
                img.height = 240;
                img.classList.add('car');

                row.appendChild(img);

                setTimeout(() => {
                    img.classList.add('visible');
                }, 100 * index); 
            } else if (ledStatus === 'ON' && existingImg) {
                existingImg.classList.remove('visible');
                setTimeout(() => {
                    row.removeChild(existingImg);
                }, 1000);
            }
        });
    });

    socket.on('disconnect', function() {
        console.log('WebSocket disconnected');
    });
});
