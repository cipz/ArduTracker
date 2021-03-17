var globalRowNumber = 1;

function addRow() {
    var x = document.getElementById('addRow');
    var container = document.createElement("div");
    container.setAttribute('id', 'row_' + globalRowNumber);
    container.innerHTML += "<hr><div class='row'>" +
        "<div class='col-sm-2'> <button type='button' onclick='deleteRow(" + globalRowNumber + ");' class='btn btn-danger btn-small'><i class='fa fa-times'></i> Delete</button></div>" +
        "<div class='col-sm-4'>" +
        "" +
        "<input type='text' class='form-control' maxlength='32' name='tkey[]' placeholder='Key' />" +
        "</div><div class='col-sm-6'>" +
        "" +
        "<textarea class='form-control' name='tval[]' placeholder='Value' rows=1></textarea>" +
        "</div></div>";
    x.appendChild(container);
    globalRowNumber++;
}

function deleteRow(i) {
    document.getElementById("row_" + i).outerHTML = "";
}

function deleteDefault(i) {
    var x = confirm('Are you really sure to delete this key-value?');
    if (x)
        document.getElementById("default_" + i).outerHTML = "";
}

function isValidJson(json) {
    try {
        JSON.parse(json);
        return true;
    } catch (e) {
        return false;
    }
}

function updateJsonStatus(response) {
    document.getElementById("saveRaw").disabled = (!response);
    document.getElementById("saveAndSendRaw").disabled = (!response);
    document.getElementById("jsonValid").innerHTML = ((response) ? "<span class='text-success'>VALID</span>" : "<span class='text-danger'>INVALID</span>");
}