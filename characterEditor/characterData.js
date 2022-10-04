function characterData() {


    this.init = function() {
        this.data = new Array(resX);
        for (let i = 0; i < resX; i++) {
            this.data[i] = new Array(resY);
            for (let j = 0; j < resY; j++) {
                this.data[i][j] = false;
            }
        }
    }


    this.toggle = function(x, y){
        this.data[x][y] = !this.data[x][y];
    }

    this.draw = function(){
        for(let x  = 0; x < resX; x++){
            for(let y = 0; y < resY; y++){

                if(this.data[x][y]){
                    fill(0);
                } else{
                    fill(255);
                }

                rect(x * sclX, y * sclY, sclX, sclY);

            }
        }
    }

    this.save = function(name){
        return this.data;
    }
}