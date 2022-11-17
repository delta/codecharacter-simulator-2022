gcc-docker:
	docker compose run --user `id -u`:`id -g` --rm simulator

