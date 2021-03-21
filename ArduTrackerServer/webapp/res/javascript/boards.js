document.getElementById("macBoardInput").addEventListener('keyup', function() {
    this.value =
        (this.value.toUpperCase()
            .replace(/[^\d|A-F]/g, '')
            .match(/.{1,2}/g) || [])
        .join(":")
});