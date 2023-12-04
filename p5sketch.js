let serial;
let latestData = "waiting for data";
let rectangleX, rectangleY, rectSize;
let rippleSize = 0; // Size of the ripple effect
let bounceSize = 0; // Variable to control bounce effect
let isBouncing = false; // State to control the bounce animation

function setup() {
  createCanvas(windowWidth, windowHeight);
  serial = new p5.SerialPort();

  serial.list();
  serial.open('/dev/tty.usbmodem11401'); // replace with your serial port

  serial.on('connected', serverConnected);
  serial.on('list', gotList);
  serial.on('data', gotData);
  serial.on('error', gotError);
  serial.on('open', gotOpen);
  serial.on('close', gotClose);
  rectangleX = width / 2;
  rectangleY = height / 2;
  rectSize = 50; // Starting size of the rectangle
}

function serverConnected() {
  print('Connected to server.');
}

function gotList(thelist) {
  print('List of Serial Ports:');
  for (let i = 0; i < thelist.length; i++) {
    print(i + ' ' + thelist[i]);
  }
}

function gotOpen() {
  print('Serial Port is open.');
}

function gotClose(){
  print('Serial Port is closed.');
  latestData = "Serial Port is Closed";
}

function gotError(theerror) {
  print(theerror);
}

function gotData() {
  let currentString = serial.readLine().trim(); // read the incoming string
  if (!currentString) return; // if the string is empty, do nothing
  latestData = currentString; // save it for the draw method

  //Collision data
    // Handle collision events
  if (latestData.startsWith('Collision')) {
    if (latestData.includes('right')) {
      rectangleX += 10;
      checkEdges();
    } else if (latestData.includes('left')) {
      rectangleX -= 10;
      checkEdges();
    } else if (latestData.includes('front')) {
      rectangleY -= 10;
      checkEdges();
    } else if (latestData.includes('back')) {
      rectangleY += 10;
      checkEdges();
    }
  }
  
  // Check for button press events and trigger animations
  if (latestData === 'Button pressed once') {
    isBouncing = true;
    bounceSize = 65; // Start the bounce effect
  } else if (latestData === 'Button double pressed') {
    rippleSize = 1; // Start the ripple effect
  }
}

function draw() {
  background(255);
  
  // If a bounce animation is ongoing, animate the bounce
  if (isBouncing) {
    rectSize = 50 + bounceSize;
    bounceSize *= 0.9; // Dampen the bounce size to make it decay
    if (abs(bounceSize) < 0.5) {
      isBouncing = false; // Stop the bounce animation when it's small enough
      rectSize = 50; // Reset the rectSize
    }
  }

  // Draw the rectangle
  rect(rectangleX - rectSize / 2, rectangleY - rectSize / 2, rectSize, rectSize);

  // Draw the ripple effect
  if (rippleSize > 0) {
    noFill();
    stroke(0);
    ellipse(rectangleX, rectangleY, rippleSize, rippleSize);
    rippleSize += 2; // Increase the ripple size
    if (rippleSize > width) {
      rippleSize = 0; // Reset the ripple size
    }
  }

  // Reset the animation if the button is held for 3 seconds
  if (latestData === 'Button held for 3 seconds') {
    resetAnimation();
  }
}

// Reset all animations
function resetAnimation() {
  rippleSize = 0;
  bounceSize = 0;
  isBouncing = false;
  rectSize = 50; // Reset the rectangle size
  rectangleX = width / 2;
  rectangleY = height / 2;
  rectSize = 50; // Starting size of the rectangle
}

// Check if the rectangle hits the edges of the canvas
function checkEdges() {
  if (rectangleX > width - rectSize / 2) {
    rectangleX = width - rectSize / 2;
  } else if (rectangleX < rectSize / 2) {
    rectangleX = rectSize / 2;
  }

  if (rectangleY > height - rectSize / 2) {
    rectangleY = height - rectSize / 2;
  } else if (rectangleY < rectSize / 2) {
    rectangleY = rectSize / 2;
  }
}
