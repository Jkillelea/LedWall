function updateHue(element) {
    console.log(element);
    sliderCallback("hueSlider", "hueSliderValue", "/hue?hue=");
}

function updateSat(element) {
    console.log(element);
    sliderCallback("satSlider", "satSliderValue", "/sat?sat=");
}

function updateBrightness(element) {
    console.log(element);
    sliderCallback("brightnessSlider", "brightnessSliderValue", "/brightness?brightness=");
}

function sliderCallback(slider, textView, httpEndpoint) {
    var sliderValue = document.getElementById(slider).value;

    document.getElementById(textView).innerHTML = sliderValue;
    console.log(sliderValue);

    var xhr = new XMLHttpRequest();
    xhr.open("GET", httpEndpoint + sliderValue, true);
    xhr.send();
}
