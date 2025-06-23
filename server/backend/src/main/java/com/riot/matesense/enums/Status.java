package com.riot.matesense.enums;

public enum Status {
    OPENED, CLOSED, UNKNOWN, NONE;

    public static Status fromCode(int code) {
        return switch (code) {
            case 0 -> CLOSED;
            case 1 -> OPENED;
            case 2 -> UNKNOWN;
            case 3 -> NONE;
            default -> null;
        };
    }
}
