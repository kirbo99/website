let exitLights = ["E1", "E2", "E3", "E4", "E5", "E6", "E7"];
let mainLights = ["L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8"];



let defaultLightColor = "#ffff00";

$(document).ready(function() {
    
    // Import SVG File
    $.get("assets/svg/home.svg", function(data) {
        $("#SVG_Object").append(data);
    }, "text").then(function() {
        $("svg").append("<path id='myPath' d='' />");

        var svg = document.querySelector("svg")

        var images = {
            "Kitchen_Fire": {
                "x": "100",
                "y": "50"
            },

            "Garage_Fire": {
                "x": "100",
                "y": "85"
            },

            "Bedroom_Fire": {
                "x": "45",
                "y": "85"
            }
        }


        Object.keys(images).forEach(image => { 

            var dim = "22.5px"
            var obj = images[image]
            var img = document.createElementNS("http://www.w3.org/2000/svg", "image");
            img.setAttribute("x", obj["x"])
            img.setAttribute("y", obj["y"])
            img.setAttribute("width", dim)
            img.setAttribute("height", dim)
            
            img.setAttribute("id", image)
            img.setAttribute("class", "fire")

            img.setAttribute("href", "assets/img/fire.png")
    
            svg.appendChild(img)
    
        })

        $(".fire").css("display", "none")

    });


    

    


    // Toggle Checkbox
    $(".checkbox").click(function() {
        var t = $(this).attr("content");

        if ($(this).is(":checked")) {
            $(`.${t}`).hide()
        } else {
            $(`.${t}`).show()
        }
    });

    
});


function resetFire() {
    $(".Fire").each(function() {
        $(this).hide();
    });
}


function resetAllLights() {
    resetFire()
    $(".light, .exitLight").each(function() {
        $(this).css("fill", defaultLightColor)
    });
}

function resetMainLights() {
    resetFire()
    $(".light").each(function() {
        $(this).css("fill", defaultLightColor)
    });
}

function resetExitLights() {
    resetFire()
    $(".exitLight").each(function() {
        $(this).css("fill", defaultLightColor)
    });
}

function findExits(locations) {

    var alarms = {}
    var lights = {}
    lights["All"] = []
    
    

    function calcDistance(a, b) {
        return Math.sqrt(Math.pow(Math.round(a, 2), 2) + Math.pow(Math.round(b, 2), 2));
    }


    $(".fireAlarm").each(function() {

        var alarm = $(this);
        var alarmID = alarm.attr("id");
        var alarmObj = document.querySelector(`#${alarmID}`);
        var alarmBBox = alarmObj.getBBox();
        var locationName = alarmID.split("_").pop();
        alarms[alarmID] = {}

        if (locations.includes(locationName)) {
            $(`#${locationName}_Fire`).show()
            alarms[alarmID]["isFire"] = true;
        } else {
            alarms[alarmID]["isFire"] = false;
        }

        var alarmX = alarmBBox.x;
        var alarmY = alarmBBox.y;
        
        var lightClasses = ["light", "exitLight"];

        lightClasses.forEach(lightType => {

            $(`.${lightType}`).each(function() {
                
                var light = $(this);
                var lightID = light.attr("id");
                var lightObj = document.querySelector(`#${lightID}`);
                var lightBBox = lightObj.getBBox();

                lights[lightID] = (lights[lightID] == undefined) ? [] : lights[lightID]
                lights["All"] = (lights[lightID] == undefined) ? [] : lights["All"]
            
            if (alarms[alarmID]["isFire"]) {

                var lX = lightBBox.x;
                var lY = lightBBox.y;

                var lightX = lX;
                var lightY = lY;
                
                if (lX < 0 && lY < 0) {
                    lightX = Math.abs(lX)
                    lightY = Math.abs(lY)
                } else if (lX < 0) {
                    lightX = lY 
                    lightY = lX * -1
                } else if (lY < 0) {
                    lightX = lY * -1
                    lightY = lX 
                } 

                var diffX = alarmX - lightX;
                var diffY = alarmY - lightY;
                
                distance = calcDistance(diffX, diffY);

                lights[lightID].push(Math.round(distance, 2))
                lights[lightID] = lights[lightID].sort();
                lights["All"].push(Math.round(distance, 2))
            }

            });
        });
    });

    var minNums = []
    Object.keys(lights).forEach(light => {
        var distances = lights[light]
        distances["min"] = Math.min(...distances)
        distances["max"] = Math.max(...distances)
        if (light != "All") {
            minNums.push(distances["min"])
        }


    });

    minNums["min"] = Math.min(...minNums)
    minNums["max"] = Math.max(...minNums)
    console.log(alarms)
    console.log(lights)

    function arrayAverage(arr) {
        sum = arr.reduce((acc, cur) => acc + cur, 0);
        return sum / arr.length;
    }

    

    var minDistance = Math.min(...lights["All"]);
    var maxDistance = Math.max(...lights["All"]);
    var average = arrayAverage(lights["All"]);
    // console.log(`ALL LIGHTS: Min: ${minDistance}, Max: ${maxDistance}, Average: ${Math.round(average)}\n`)



    Object.keys(lights).forEach(light => {

        if (light == "All") {
            return
        }

        var green = "#32ad10"
        var orange = "#de660b"
        var red = "#8a0d08"

        var distances = lights[light]

        var lightCount = distances.length

        var minDist = Math.min(...distances)
        var lightMax = Math.max(...distances)
        var lightAvg = arrayAverage(distances)
        

        var range = minNums['max'] - minNums['min'] ;

        
        var thirdOfRange = range / 3;
        var orangeMinRange = thirdOfRange;
        var greenMinRange = thirdOfRange * 2.5;

        // console.log(`${light}: Min: ${minDist}, Max: ${lightMax}, Average: ${Math.round(lightAvg)}, Light Count ${lightCount}`)

        let color = red;

        if (minDist > orangeMinRange) { color = orange }
        if (minDist >= greenMinRange) { color = green }

        if (alarms["Alarm_Garage"].isFire && light == "E3") {
            color = red
        }

        $(`#${light}`).css("fill", color)

    });

}



function fireAlarm(location) {

    resetAllLights()
    findExits(location)

}