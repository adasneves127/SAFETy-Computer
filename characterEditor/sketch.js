let sclX, sclY, resX, resY;
let character = new characterData();
let charName
let characterNames = []
let nameBox

let characterMap = {};

function setup() {

  resX = 4;
  resY = 8;
  sclX = 30;
  sclY = 30


  resX++;
  resY;

  createCanvas(resX * sclX, resY * sclY);
  character.init();

  let reset = createButton('Clear');

  reset.mousePressed(function() {
    character.init();
  });

  let convert = createButton('Convert');
  convert.mousePressed(convertChar);

  let save = createButton('Save');
  save.mousePressed(saveChar);

  createElement('br');
  createElement('br');
  
  charName = createInput('');

  let downloadButton = createButton('Download');
  downloadButton.mousePressed(function(){
    download(JSON.stringify(characterMap), "characterMap.json", "json")
  })

  nameBox = createDiv("Characters:<br>");
  nameBox.id("nameBox");


}

function drawChar(charArr){
  character.init();
  for(let i = 0; i < charArr.length; i++){
    for(let j = 0; j < 8; j++){
      if(charArr[i] & (1 << j)){
      character.toggle(i, j)
      }
    }
  }
}

function saveChar(){

  let data = convertChar()
  characterMap[charName.value()] = data;
  characterNames.push(charName.value());
  print(characterMap);
}

function convertChar(){
  let data = []
  for( let x = 0; x < resX; x++){
    let charData = "0b";
    for (let i = resY - 1; i >= 0 ; i--) {
      charData += character.data[x][i] ? "1" : "0";
    }
    data.push(charData);
  }
  
  

  return data;


}

function draw() {
  background(220);
  //drawGrid();
  character.draw();

  let box = document.getElementById("nameBox");

  for(let i = 0; i < characterNames.length; i++){
    characterNames[i] = characterNames[i].toUpperCase();
  }

  box.innerHTML = "Characters:<br>" + characterNames.join("<br>");

}


function drawGrid(){
  for (let i = 0; i < resX; i++) {
    for (let j = 0; j < resY; j++) {
      let x = i * scl;
      let y = j * scl;
      stroke(0);
      strokeWeight(1);
      line(x, 0, x, height);
      line(0, y, width, y);
    }
  }
}

function mouseClicked(){
  try{
    let x = floor(mouseX / sclX);
    let y = floor(mouseY / sclY);
    character.toggle(x, y);
  }
  finally{
    return;
  }
}


function download(data, filename, type) {
  var file = new Blob([data], {type: type});
  if (window.navigator.msSaveOrOpenBlob) // IE10+
      window.navigator.msSaveOrOpenBlob(file, filename);
  else { // Others
      var a = document.createElement("a"),
              url = URL.createObjectURL(file);
      a.href = url;
      a.download = filename;
      document.body.appendChild(a);
      a.click();
      setTimeout(function() {
          document.body.removeChild(a);
          window.URL.revokeObjectURL(url);  
      }, 0); 
  }
}