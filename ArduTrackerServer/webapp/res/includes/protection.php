<?php

class protection
{
    private const SINGLE_PASSWORD = PASSWORD_PROTECTION;
    private const SESSION_NAME = 'ArduTrack';
    private bool $status;
    private bool $protected;

    private function updateStatus()
    {
        $this->status = isset($_SESSION[self::SESSION_NAME]);
    }

    public function __construct(bool $protected = false)
    {
        $this->updateStatus();
        $this->protected = $protected;
        $this->checkCredentials();
    }

    public function isLoggedIn() {
        return $this->status;
    }

    public function getSessionContent() {
        return $this->status ? $_SESSION[self::SESSION_NAME] : '';
    }

    public function getProtected() {
        return $this->protected;
    }

    public function setProtected(bool $val) {
        $this->protected = $val;
    }

    public function checkCredentials()
    {
        if(!$this->status && $this->protected) {
            header('Location: ' . LOGIN_PAGE_URL);
        }
    }

    public function accessWithSinglePassword(string $password)
    {
        if ($password == self::SINGLE_PASSWORD) {
            $_SESSION[self::SESSION_NAME] = time();
            $this->updateStatus();
            return true;
        } else {
            return false;
        }
    }

    public function logout() {
        session_unset();
        return true;
    }
}
