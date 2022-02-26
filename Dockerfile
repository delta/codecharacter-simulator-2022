FROM gcr.io/distroless/cc-debian11

COPY ./build/bin/main /main

ENTRYPOINT [ "/main" ]
